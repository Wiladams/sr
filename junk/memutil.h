#include <stdint.h>

extern void *amalloc (size_t bytes);
extern void afree (void *mem);
extern size_t amsizeof (void *mem);
extern void *acalloc (size_t ele, size_t num);
extern void *arealloc (void *mem, size_t size);

