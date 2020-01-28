

#include "memutil.h"
#include "arena.h"

void *amalloc (size_t bytes)
{
size_t *ans = valloc (bytes + sizeof(size_t));

    *ans++ = bytes;
    return ans;
}

void afree (void *mem)
{
size_t *size;

    if( size = (size_t *)mem )
        size--, vfree (size, *size + sizeof(size_t));
}

size_t amsizeof (void *mem)
{
size_t *size;

    if( size = (size_t *)mem )
        return *--size;

    return 0;
}

void *acalloc (size_t ele, size_t num)
{
size_t xtra;
void *ans;

    if( xtra = ele % sizeof(unsigned) - 1 )
        ele += sizeof(unsigned) - xtra;

    ans = amalloc (ele * num);
    memset (ans, 0, ele * num);
    return ans;
}

void *arealloc (void *mem, size_t size)
{
size_t *old;
void *ans;

    if( old = (size_t *)mem )
        if( *--old >= size )
            return vtrim (old, *old, size), *old = size, mem;

    ans = amalloc (size);

    if( old )
        memcpy (ans, mem, *old), vfree (old, *old + sizeof(size_t));

    return ans;
}
