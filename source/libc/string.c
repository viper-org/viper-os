#include <string.h>

void *memset(void *d, int ch, size_t sz)
{
    char *p = d;
    while (sz)
    {
        *p = ch;
        ++p;
        --sz;
    }
    return d;
}
