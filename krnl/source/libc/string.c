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

int memcmp(const void *a, const void *b, size_t sz)
{
    const char *x = a;
    const char *y = b;
    for (size_t i = 0; i < sz; ++i)
    {
        if (x[i] != y[i]) return x[i] - y[i];
    }
    return 0;
}

void *memmove(void *d, const void *s, size_t sz)
{
    char *x = d;
    const char *y = s;
    
    if(y < x && y + sz > x)
    {
        y += sz;
        x += sz;
        while(sz-- > 0)
            *--x = *--y;
    }
    else
    {
        while(sz-- > 0)
            *x++ = *y++;
    }
    
    return d;
}

size_t strlen(const char *s)
{
    if (!s)
    {
        int a = 1;
    }
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

char *strcat(char *d, const char *s)
{
    char *p = d;
    while (*p) ++p;

    while (*s)
        *p++ = *s++;

    *p = 0;
    return d;
}

char *strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while (*s2)
    {
        *s1 = *s2;
        ++s1;
        ++s2;
    }
    *s1 = 0;
    return s;
}

char *strdup(const char *s)
{
    size_t sz = strlen(s) + 1;
    char *mem = kheap_alloc(sz);
    for (size_t i = 0; i < sz; ++i)
        mem[i] = s[i];
    return mem;
}

char *strrchr(const char *s, int ch)
{
    int sz = strlen(s);
    for (int i = sz; i >= 0; --i)
    {
        if (s[i] == ch) return &s[i];
    }
    return 0;
}
