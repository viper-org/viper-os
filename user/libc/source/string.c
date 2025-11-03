#include <string.h>

void *memset(void *d, int ch, size_t size)
{
    unsigned char *p = d;
    while (size--)
    {
        *p = ch;
        ++p;
    }
    return d;
}
