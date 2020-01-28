#include "arena.h"

Page *PageLIFO;

// allocate and initialize a new page frame for arenas

Page *vframe (void)
{
unsigned xtra, size = FRAME_SIZE;
Page *page;

#ifdef unix
    page = (Page *)sbrk (size);

    //  round brk value to page boundary

    if( xtra = (unsigned)page & 0xfff )
        sbrk (0x1000 - xtra), size += 0x1000 - xtra;
#else
    page = (Page *)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#endif
    page->first = page->size = size;
    page->prev = PageLIFO;

    PageLIFO = page;
    return page;
}

//  arena templates for three arena shapes, but feel
//  free to add others to suit your own needs.

//  overlap between blkmax and blksize can reduce
//  the percentage of granularity induced waste,
//  as illustrated in the 16 byte and 128 byte
//  templates below.

typedef struct {
    unsigned blksize;  //  allocation granularity
    unsigned blkcnt;   //  number of blocks in the arena
    unsigned blkmax;   //  maximum allocation to support
    Arena *arena;      //  chain of arenas with template
    Arena *scan;       //  current arena being allocated
} Template;

Template ArenaDefs[] = {
    {16, 8192, 511 },       //  16 x 8K  = 512K total
    {128, 4096, 4095 },     //  128 x 4K = 512K total
    {4096, 512, 65535 },    //  4K x 512 =  2MB total
    {0}                     //  64K and above
};

//  allocate and initialize a new arena
//  marking an initial allocation

void *varena (size_t bytes, Template *tmpl)
{
unsigned xtra, blks, bits, blkmax, blksize, mapmax, mapbytes;
unsigned *map;
Arena *arena;
Page *page;

    assert (bytes > 0);

    //  build the arena from a table template,
    //  or make a huge arena with two blocks
    //  of one half the total request each

    if( blksize = tmpl->blksize ) {
        blkmax = tmpl->blkmax;
        blks = tmpl->blkcnt;
    } else {
        if( xtra = bytes & 0xfff )   // round for 4K virtual memory
            bytes += 4096 - xtra;

        assert (~bytes & 1);
        blksize = bytes / 2;
        blkmax = bytes - 1;
        blks = 2;
    }

    //  block count for the initial allocation

    assert (blksize > 0);

    bits = (bytes + blksize - 1);
    bits /= blksize;

    //  round mapmax up to unsigned size multiple
    //  unless doing a huge block without map bits

    assert (MAP_BITS > 2);

    if( mapmax = blks / MAP_BITS )
      if( blks & (MAP_BITS - 1) )
        mapmax++;

    mapbytes = mapmax * sizeof(unsigned);

    //  allocate new page on overflow of current frame
    //  or on startup

    if( page = PageLIFO ) {
      if( page->first < mapbytes + sizeof(Page) + sizeof(Arena) )
        page = vframe();
    } else
        page = vframe();

    //  allocate the arena in the frame

    page->first -= mapbytes;
    map = (unsigned *)((char *)page + page->first);

    page->first -= sizeof(Arena);
    arena = (Arena *)((char *)page + page->first);

    assert (page->first >= sizeof(Page));

    //  initialize and insert arena into template chain

    memset (arena, 0, sizeof(Arena));
    arena->avail = blks * blksize;
    arena->blksize = blksize;
    arena->mapmax = mapmax;
    arena->blkmax = blkmax;

    arena->next = tmpl->arena;
    tmpl->arena = arena;

    if( !arena->next )
        tmpl->scan = arena;

    //  allocate the arena's memory block

#ifdef unix
    arena->mem = sbrk (arena->avail);

    //  align allocation on page boundary

    if( xtra = (unsigned)arena->mem & 0xfff )
        sbrk (0x1000 - xtra), arena->mem += 0x1000 - xtra;
#else
    arena->mem = VirtualAlloc(NULL, arena->avail, MEM_COMMIT, PAGE_READWRITE);
#endif

    arena->end = arena->mem + arena->avail;
    arena->avail -= bits * blksize;

    if( mapbytes * CHAR_BIT < bits )
        return arena->mem;

    //  clear the initial allocation of bits in the map

    while( bits >= MAP_BITS )
        *map++ = 0, bits -= MAP_BITS, mapmax--;

    //  and mark the rest of the map as available

    if( mapmax-- )
        *map++ = ~0U << bits;
    else
        return arena->mem;

    while( mapmax-- )
        *map++ = ~0U;

    return arena->mem;
}

//  mark a partial sub-byte map allocation of 7 or fewer blocks
//  of non rightmost blocks

void *vmarkbyte (Arena *arena, unsigned char val, unsigned blks, unsigned bit)
{
unsigned *map = (unsigned *)(arena + 1) + arena->scan;
unsigned char mask = UCHAR_MAX >> (CHAR_BIT - blks);
unsigned block;

    assert (blks && blks < CHAR_BIT);
    assert (val);

    //  find the available run of ones in the byte
    //  ignoring the rightmost bit, and assuming
    //  the map byte is non zero

    do val >>= 1, bit++, assert (bit < MAP_BITS);
    while( val & mask ^ mask );

    //  clear the allocation bits

    arena->avail -= blks * arena->blksize;

    *map ^= (unsigned)mask << bit;
    assert(!(*map & (unsigned)mask << bit));

    block = arena->scan * MAP_BITS + bit;
    return arena->mem + block * arena->blksize;
}

//  allocate consecutive run of blocks from the map
//  blks must be at least one

void *vmarkmap (Arena *arena, unsigned run, unsigned blks, unsigned bit)
{
unsigned *map = (unsigned *)(arena + 1) + arena->scan, mask;
char *mem = arena->mem;
unsigned block;

    block = arena->scan * MAP_BITS + bit - run;
    arena->avail -= blks * arena->blksize;
    mem += block * arena->blksize;

    //  clear initial bits of hightest map allocation

    blks += bit - run;
    assert (blks > 0);

    mask = ~0U >> MAP_BITS - blks;

    if( run < bit ) {
      bit -= run;
      *map ^= mask ^= ((1 << bit) - 1);
      assert (!(*map & mask));
      return mem;
    }

    *map-- ^= mask;
    assert (!(map[1] & mask));
    run -= bit;

    //  clear preceeding run bits from map

    while( run >= MAP_BITS )
        assert (*map == ~0U), *map-- = 0, run -= MAP_BITS;

    assert ((*map | ~0U >> run) == ~0U);
    *map &= ~0U >> run;
    return mem;
}

//  scan tables built for contents of map bytes

#if CHAR_BIT != 8

// you'll have to construct your own tables

#else

//  new run value: consecutive left side bits
//  (zero high order bit means no run to left)
//  (table also removes low order bit)

unsigned char ArenaRun[64] = {
1, 1, 1, 1, 1, 1, 1, 1, // 0x80 - 0x8f
1, 1, 1, 1, 1, 1, 1, 1, // 0x90 - 0x9f
1, 1, 1, 1, 1, 1, 1, 1, // 0xA0 - 0xAf
1, 1, 1, 1, 1, 1, 1, 1, // 0xB0 - 0xBf
2, 2, 2, 2, 2, 2, 2, 2, // 0xC0 - 0xCf
2, 2, 2, 2, 2, 2, 2, 2, // 0xD0 - 0xDf
3, 3, 3, 3, 3, 3, 3, 3, // 0xE0 - 0xEf
4, 4, 4, 4, 5, 5, 6, 7, // 0xF0 - 0xFf
};

// available bits to continue existing run of consecutive blocks
//    (entries w/high order bit set are folded onto these values)
//    (low order bit is removed, and 0xFF is also removed)

unsigned char ArenaGlom[64] = {
1, 2, 1, 3, 1, 2, 1, 4, // 0x00 - 0x0f
1, 2, 1, 3, 1, 2, 1, 5, // 0x10 - 0x1f
1, 2, 1, 3, 1, 2, 1, 4, // 0x20 - 0x2f
1, 2, 1, 3, 1, 2, 1, 6, // 0x30 - 0x3f
1, 2, 1, 3, 1, 2, 1, 4, // 0x40 - 0x4f
1, 2, 1, 3, 1, 2, 1, 5, // 0x50 - 0x5f
1, 2, 1, 3, 1, 2, 1, 4, // 0x60 - 0x6f
1, 2, 1, 3, 1, 2, 1, 7, // 0x70 - 0x7f
};

// available bits remaining after first zero
// (without redundant low order bit)

unsigned char ArenaAfter[128] = {
0, 1, 1, 2, 1, 1, 2, 3, // 0x00 - 0x0f
1, 1, 1, 2, 2, 2, 3, 4, // 0x10 - 0x1f
1, 1, 1, 2, 1, 1, 2, 3, // 0x20 - 0x2f
2, 2, 2, 2, 3, 3, 4, 5, // 0x30 - 0x3f
1, 1, 1, 2, 1, 1, 2, 3, // 0x40 - 0x4f
1, 1, 1, 2, 2, 2, 3, 4, // 0x50 - 0x5f
2, 2, 2, 2, 2, 2, 2, 3, // 0x60 - 0x6f
3, 3, 3, 3, 4, 4, 5, 6, // 0x70 - 0x7f
1, 1, 1, 2, 1, 1, 2, 3, // 0x80 - 0x8f
1, 1, 1, 2, 2, 2, 3, 4, // 0x90 - 0x9f
1, 1, 1, 2, 1, 1, 2, 3, // 0xA0 - 0xAf
2, 2, 2, 2, 3, 3, 4, 5, // 0xB0 - 0xBf
2, 2, 2, 2, 2, 2, 2, 3, // 0xC0 - 0xCf
2, 2, 2, 2, 2, 2, 3, 4, // 0xD0 - 0xDf
3, 3, 3, 3, 3, 3, 3, 3, // 0xE0 - 0xEf
4, 4, 4, 4, 5, 5, 6, 7, // 0xF0 - 0xFf
};
#endif

//  scan an existing arena for available space
//  processing the map in byte size pieces
//  of int sized chunks using the tables

void *vscan (Arena *arena, size_t bytes)
{
unsigned chunk, *map = (unsigned *)(arena + 1);
unsigned run = 0, blks, bit;
unsigned char nxt;

  blks = bytes + arena->blksize - 1;
  assert (arena->blksize > 0);

  if( blks /= arena->blksize )
    do if( chunk = map[arena->scan] )
     for( bit = 0; bit < MAP_BITS; bit += CHAR_BIT )
      if( nxt = chunk >> bit )    // next byte of chunk
       if( nxt < UCHAR_MAX )      // less than 8 blocks available ???
        if( ~nxt & 1 || run + ArenaGlom[nxt/2 & 0x3f] < blks )
          if( ArenaAfter[nxt/2] < blks ) // bits available in byte
            if( nxt & 0x80 )      // establish new run
              run = ArenaRun[nxt/2 & 0x3f];
            else
              run = 0;  //  no run possible without the leading bit
          else          // request of fewer than 8 blocks will fit
            return vmarkbyte (arena, nxt, blks, bit);
        else    // 8 blocks or a run spanning two or more map bytes
          return vmarkmap (arena, run, blks, bit);
       else if( run + CHAR_BIT < blks )
        run += CHAR_BIT;// 8 more blocks still not enough
       else             // or, run now fits
        return vmarkmap (arena, run, blks, bit);
      else
        run = 0;        // byte is all zero bits
     else
        run = 0;        // chunk is all zero bits
    while( ++arena->scan < arena->mapmax );

    arena->scan = 0;    // next time start scan from the beginning
    return NULL;
}

//  allocate a new block of size bytes

void *valloc (size_t bytes)
{
Template *tmpl = ArenaDefs;
unsigned blkmax, doit = 2;
Arena *start, *first;
void *mem;

    //  round request up to smallest Template blocksize

    assert (tmpl->blksize > 0);

    if( bytes < tmpl->blksize )
        bytes = tmpl->blksize;

    //  find a suitable template, or default to the
    //  huge-block template at the end of the table

    while( blkmax = tmpl->blkmax )
      if( bytes > blkmax )
        tmpl++;
      else
        break;

    //  scan existing arenas built under the template
    //  for available space, going through each arena
    //  once before giving up and building a new arena,
    //  but do the current allocating arena twice

    if( first = tmpl->arena )
     if( start = tmpl->scan ) do
      if( tmpl->scan == start && !doit-- )
        break;
      else if( bytes < tmpl->scan->blksize )
        continue;
      else if( bytes > tmpl->scan->avail )
        continue;
      else if( tmpl->scan->mapmax )  // scan blocks with maps
        if( mem = vscan(tmpl->scan, bytes) )
            return mem;
        else
          continue;
      else  // huge arena, allocate by clearing available space
        return tmpl->scan->avail = 0, tmpl->scan->mem;
    while( tmpl->scan = tmpl->scan->next ? tmpl->scan->next : first );

    //  build a new arena and allocate space there

    return varena (bytes, tmpl);
}

//  starting with block number, set the
//  map bits to make blocks available

void vsetmap (Arena *arena, unsigned block, unsigned blks)
{
unsigned mask, *map = (unsigned *)(arena + 1) + block / MAP_BITS;
unsigned max = arena->mapmax * MAP_BITS;

    assert (blks <= max - block);
    assert (block <= max);
    assert (blks > 0);

    if( block > max )         // ensure sanity of starting block no.
        block = max;

    if( blks > max - block )  // ditto for the block count
        blks = max - block;

    arena->avail += blks * arena->blksize;

    //  set block available bits in lowest map entry of the run
    //  (these are high order bits)

    if( blks < MAP_BITS )     // less than full int of bits?
        mask = ~0U >> (MAP_BITS - blks);
    else
        mask = ~0U;

    block &= MAP_BITS - 1;

    assert (!(*map & mask << block));
    *map++ |= mask << block;

    //  calculate number of blks remaining

    if( blks > MAP_BITS - block )
        blks -= MAP_BITS - block;
    else
        return;

    //  set all block available bits for intermediate map blocks

    while( blks > MAP_BITS )
      assert (!*map), *map++ = ~0U, blks -= MAP_BITS;

    //  set block available bits in highest map block
    //  (these are low order bits)

    assert (!(*map & ~0U >> (MAP_BITS - blks)));
    *map |= ~0U >> (MAP_BITS - blks);
}

//  release memory blocks into an arena

void vfree (void *mem, size_t bytes)
{
unsigned block, entry, blks;
Arena *arena;
Page *page;

    //  round request up to smallest Template blocksize

    assert (ArenaDefs->blksize > 0);

    if( bytes < ArenaDefs->blksize )
        bytes = ArenaDefs->blksize;

    //  locate the correct arena
    //  by scanning all existing
    //  arena memory ranges

    assert (PageLIFO);

    if( page = PageLIFO )
        entry = page->first;
    else
        return;

    while( 1 )
      if( entry < page->size )
        if( arena = (Arena *)((char *)page + entry), (char *)mem < arena->mem )
            entry += sizeof(Arena) + sizeof(unsigned) * arena->mapmax;
        else if( (char *)mem < arena->end )
            break;
        else
            entry += sizeof(Arena) + sizeof(unsigned) * arena->mapmax;
      else if( assert(page->prev), page = page->prev )
          entry = page->first;
      else
          return;

    //  huge blocks have no map, so we just mark
    //  the entire arena available and return

    if( !arena->mapmax ) {
        arena->avail = arena->blkmax + 1;
        return;
    }

    //  calculate the number of blocks in the request

    assert (arena->blksize > 0);

    blks = bytes + arena->blksize - 1;
    blks /= arena->blksize;

    //  calculate the starting block number
    //  and set the available bits

    block = ((char *)mem - (char *)arena->mem) / arena->blksize;
    vsetmap (arena, block, blks);
}

//  trim memory block to smaller new size
//  blks is oldsize, bytes is newsize

void vtrim (void *mem, size_t blks, size_t bytes)
{
unsigned block, entry;
Arena *arena;
Page *page;

    assert (blks >= bytes);

    //  round sizes up to smallest Template blocksize

    assert (ArenaDefs->blksize > 0);

    if( blks < ArenaDefs->blksize )
      blks = ArenaDefs->blksize;

    if( bytes < ArenaDefs->blksize )
      bytes = ArenaDefs->blksize;

    //  locate the correct arena by scanning
    //  all existing arena memory ranges

    assert (PageLIFO);

    if( page = PageLIFO )
      entry = page->first;
    else
      return;

    while( 1 )
     if( entry < page->size )
      if( arena = (Arena *)((char *)page + entry), (char *)mem < arena->mem )
        entry += sizeof(Arena) + sizeof(unsigned) * arena->mapmax;
      else if( (char *)mem < arena->end )
        break;
      else
        entry += sizeof(Arena) + sizeof(unsigned) * arena->mapmax;
     else if( assert (page->prev), page = page->prev )
        entry = page->first;
     else
        return;

    //  huge blocks have no map; do nothing to resize these.

    if( !arena->mapmax )
      return;

    //  calculate the number of blocks in the new size

    assert (arena->blksize > 0);

    bytes += arena->blksize - 1;
    bytes /= arena->blksize;

    //  calculate the number of blocks in the original block, the
    //  starting block number, and the change in block counts

    block = ((char *)mem - (char *)arena->mem) / arena->blksize;
    blks += arena->blksize - 1;
    blks /= arena->blksize;

    //  free extra blocks

    if( blks > bytes )
      vsetmap (arena, block + bytes, blks - bytes);
}




 