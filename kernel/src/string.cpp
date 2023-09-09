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