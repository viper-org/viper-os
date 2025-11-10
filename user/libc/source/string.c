#include <string.h>

void *memcpy(void *d, const void *s, size_t size)
{
    char *d1 = d;
    const char *s1 = s;
    while(size--)
    {
        *d1 = *s1;
        ++d1;
        ++s1;
    }
    return d;
}

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


int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

size_t strlen(const char *s)
{
    size_t i = 0;
    while (s[i]) ++i;
    return i;
}
