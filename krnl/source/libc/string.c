#include <string.h>

#include "mm/kheap.h"

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

void *memcpy(void *d, const void *s, size_t sz)
{
    char *a = d;
    const char *b = s;
    for (size_t i = 0; i < sz; ++i)
    {
        a[i] = b[i];
    }
    return d;
}

size_t strlen(char *s)
{
    size_t i = 0;
    while (s[i]) ++i;
    return i;
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

char *strdup(char *s)
{
    size_t sz = strlen(s) + 1;
    char *mem = kheap_alloc(sz);
    for (size_t i = 0; i < sz; ++i)
        mem[i] = s[i];
    return mem;
}
