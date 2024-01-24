#include <stdlib.h>

void *libc_alloc_bytes(size_t);
void *malloc(size_t size)
{
    return libc_alloc_bytes(size);
}