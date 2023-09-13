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

int memcmp(const void* p1, const void* p2, size_t count)
{
    const char* m1 = (const char*)p1;
    const char* m2 = (const char*)p2;
    for (size_t i = 0; i < count; i++, m1++, m2++)
    {
        if (*m1 < *m2)
        {
            return -1;
        }
        if (*m1 > *m2)
        {
            return 1;
        }
    }
    return 0;
}

void memcpy(void* dest, const void* src, size_t count)
{
    char* dst = (char*)dest;
    const char* source = (const char*)src;
    while(count--)
    {
        *(dst++) = *(source++);
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

void strcpy(char* s1, const char* s2)
{
    while(*s2)
    {
        *s1 = *s2;
        s1++;
        s2++;
    }
}

size_t strlen(const char* str)
{
    size_t ret = 0;
    while(str[ret])
    {
        ret++;
    }
    return ret;
}