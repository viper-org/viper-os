#include <stdlib.h>

void libc_free_bytes(void *);
void free(void *ptr)
{
    return libc_free_bytes(ptr);
}