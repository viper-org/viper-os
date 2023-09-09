#ifndef VIPER_OS_MM_HEAP_H
#define VIPER_OS_MM_HEAP_H 1

#include <stddef.h>

namespace mm
{
    void  Init();

    void* Alloc(size_t count);
    
    void  Free(void* addr);
}

#endif