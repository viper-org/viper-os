#include <string.h>


void* memset(void* d, int c, size_t sz)
{
    unsigned char* d1 = reinterpret_cast<unsigned char*>(d);
    for (size_t i = 0; i < sz; ++i)
    {
        d1[i] = c;
    }
    return d;
}