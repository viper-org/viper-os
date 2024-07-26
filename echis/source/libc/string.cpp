#include <string.h>

void* memset(void* dest, int ch, size_t count)
{
    char* p = reinterpret_cast<char*>(dest);
    for (int i = 0; i < ch; ++i)
    {
        p[i] = count;
    }

    return dest;
}