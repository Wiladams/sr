

#include <stdio.h>
#include "arena.h"



char *Buff[5000];
char Line[80000];

void blkaddrs (Arena *arena, unsigned idx)
{
char *mem = arena->mem + idx * MAP_BITS * arena->blksize;
unsigned *map = (unsigned *)(arena + 1) + idx;
unsigned bit;

 for( bit = 0; bit < MAP_BITS; bit++ )
  if( !(*map >> bit & 1) )
   fprintf (stderr, "block %x not freed\n", (unsigned int)(mem + bit * arena->blksize));
}

int main (int argc, char **argv)
{
    int usemalloc = 0;
    int check = 0;
    int count = 0;
    int debug = 0;
    int len, idx;
    FILE *Out = stdout;
    FILE *In = stdin;

puts(stdout);

    //  process any program options present

    if( argc > 1 )
      if( argv[1][0] == '-' )
        for( argc--, argv++; *++(*argv); )
          switch( **argv | 0x20 ) {
          case 'm': usemalloc++;    break;
          case 'd': debug++;        break;
          case 'c': check++;        break;
          }

    //  open input and output files if present

    if( argc > 1 )
      if( !(In = fopen (argv[1], "r")) )
        return 1;

    if( argc > 2 )
      if( !(Out = fopen (argv[2], "w")) )
        return 1;

    // scramble the input as the test plan

    while( fgets (Line, sizeof(Line), In) ) {
      if( count < 5000 )
        idx = count++;
      else {
        idx = (unsigned)rand() % count;
        len = strlen(Buff[idx]);

        if( debug )
          fprintf(stderr, "free %.6d %x\n", len + 1, Buff[idx]);

        fwrite (Buff[idx], len, 1, Out);
        usemalloc ? free (Buff[idx]) : vfree (Buff[idx], len + 1);
      }

      len = strlen (Line);
      Buff[idx] = usemalloc ? malloc (len + 1) : valloc (len + 1);

      if( debug )
        fprintf(stderr, "req  %.6d %x\n", len + 1, Buff[idx]);

      memcpy (Buff[idx], Line, len + 1);
    }

    while( count-- ) {
        len = strlen (Buff[count]);
        fwrite (Buff[count], len, 1, Out);
        usemalloc ? free (Buff[count]) : vfree (Buff[count], len + 1);

        if( debug )
          fprintf(stderr, "free %.6d %x\n", len + 1, Buff[count]);
    }

    fclose (Out);
    fclose (In);

    //  ensure all bits were reset in all arenas

    if( check ) {
    unsigned entry, idx;
    Arena *arena;
    Page *page;

      if( page = PageLIFO ) do
        for( entry = page->first; entry < page->size; entry += sizeof(Arena) + arena->mapmax * sizeof(unsigned) )
          for( arena = (Arena *)((char *)page + entry), idx = 0; idx < arena->mapmax; idx++ )
            if( ((unsigned *)(arena + 1))[idx] != ~0U )
                blkaddrs(arena, idx);
      while( page = page->prev );
    }
}

