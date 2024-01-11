#include <string.h>

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