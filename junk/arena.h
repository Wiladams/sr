//  bit map arena virtual memory allocator
//  using one bit of overhead per block

//  When releasing memory, you must supply the block size. Under
//  modern programming discipline, this should be available since
//  you generally have to check OBJECT SIZE FOR OVERFLOW anyway.

//  A standard malloc and free routine is conditionally supplied
//  that remembers the block sizes if this is a problem. 

//  please report bugs located to the program author,
//  karl_m@acm.org www.geocities.com/malbrain

//  n.b. the semantics of valloc differ slightly from the standard
//  unix V R4 definition:  the memory allocated is aligned to
//  the blocksize of the appropriate templated arena, not
//  necessarily to the virtual memory system page size.
//  Also, valloc requests must be released with vfree.

//  That said, with the default templates given below,
//  valloc requests for 4K or more will align
//  on a page boundary.

#include <limits.h>
#include <stdlib.h>

#define NDEBUG
#include "assert.h"

#ifdef unix
#include <unistd.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

//  each memory arena structure is followed by a bit map
//  allocation table made of unsigned ints where the low
//  order bit represents the lowest block number.

typedef struct Arena_ {
    char *mem, *end;    // beginning and end of arena allocation
    void *next;         // next arena in chain with same template

    unsigned blksize;   // block size for memory allocation
    unsigned mapmax;    // map size in ints immediately following
    unsigned blkmax;    // maximum block allocation in bytes
    unsigned avail;     // amount of available arena space
    unsigned scan;      // current scan offset
} Arena;

//  the Arenas are arrayed with their bit maps into page frames;
//  the first structure in the page frame describes the page
//  new arenas are allocated from the end

typedef struct Page_ {
    unsigned first;     // offset of lowest arena allocated
    unsigned size;      // size of this page in bytes
    void *prev;         // previous page of arenas in chain
} Page;

extern Page *PageLIFO;

#define MAP_BITS (CHAR_BIT * sizeof(unsigned))
#define FRAME_SIZE 8192


// Function interface
extern void *valloc (size_t bytes);
extern void vfree (void *mem, size_t bytes);
extern void vtrim (void *mem, size_t blks, size_t bytes);
