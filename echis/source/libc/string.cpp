#include <string.h>

void* memset(void* dest, int ch, size_t count)
{
    char* p = reinterpret_cast<char*>(dest);
    for (size_t i = 0; i < count; ++i)
    {
        p[i] = ch;
    }

    return p;
}

void* memcpy(void* dest, const void* src, size_t count)
{
    char* p1 = reinterpret_cast<char*>(dest);
    const char* p2 = reinterpret_cast<const char*>(src);

    while(count--)
    {
        *p1 = *p2;
        ++p1;
        ++p2;
    }

    return dest;
}

int strcmp(const char* lhs, const char* rhs)
{
    while (*lhs && (*lhs == *rhs))
    {
        ++lhs;
        ++rhs;
    }

    return *lhs - *rhs;
}

size_t strlen(const char* s)
{
    size_t l = 0;
    while(s[l]) ++l;
    return l;
}