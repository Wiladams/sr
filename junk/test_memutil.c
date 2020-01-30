#include <stdio.h>
#include "memutil.h"

int main(int argc, char **argv)
{
    // allocate some memory
    char * m = (char *)amalloc(3200);

    // check it's size
    size_t sz = amsizeof(m);
    printf("Size: %d\n", sz);

    // free it
    afree(m);

    return 0;
}