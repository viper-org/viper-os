#include <string.h>

void* memset(void* s, int c, size_t n)
{
    char* p = s;
    for (size_t i = 0; i < n; ++i)
    {
        p[i] = c;
    }
    return s;
}

void* memcpy(void* dest, const void* src, size_t count)
{
    char* d = dest;
    const char* s = src;
    for(size_t i = 0; i < count; ++i)
    {
        d[i] = s[i];
    }
    return dest;
}