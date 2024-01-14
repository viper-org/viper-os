#include <string.h>

size_t strlen(const char* s)
{
    size_t ret = 0;
    while(s[ret])
    {
        ++ret;
    }
    return ret;
}

void* memset(void* dest, int ch, size_t count)
{
    char* destPtr = (char*)dest;
    while(count--)
    {
        *destPtr = ch;
        destPtr++;
    }
    return dest;
}

void memcpy(void* dest, const void* src, size_t count)
{
    char* p1       = static_cast<char*>(dest);
    const char* p2 = static_cast<const char*>(src);

    for (size_t i = 0; i < count; ++i)
    {
        p1[i] = p2[i];
    }
}

void strcpy(char* dest, const char* src)
{
    while(*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

int memcmp(const void* a, const void* b, size_t count)
{
    const char* p1 = reinterpret_cast<const char*>(a);
    const char* p2 = reinterpret_cast<const char*>(b);
    for (size_t i = 0; i < count; ++i, ++p1, ++p2)
    {
        if (*p1 < *p2)
        {
            return -1;
        }
        if (*p1 > *p2)
        {
            return 1;
        }
    }
    return 0;
}