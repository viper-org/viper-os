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


size_t strlen(const char *s)
{
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}
