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