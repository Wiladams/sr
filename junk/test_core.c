#include <stdio.h>
#include "arena.h"

size_t msizeof (void *mem)
{
    if (mem != NULL) {
        size_t *size = (size_t *)mem;
        size = size - 1;
        return *size;

    }

    return 0;
}

int main(int argc, char **argv)
{
    size_t msize = 1024;
    printf("sizeof(size_t): %d\n", (int)sizeof(msize));

    // allocate some memory
    char * m = (char *)valloc(msize);
    printf("valloc(1024): %p\n", m);

    // Sizeof
    size_t mesize = msizeof(m);
    //printf("msizeof: %d\n", (int)msize);

    // trim the size
    vtrim(m, msize, msize / 2);

    // free the memory
    vfree(m, msize);

    return 0;
}