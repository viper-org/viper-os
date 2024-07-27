#include <new>

#include <echis/mm/heap.h>

void* operator new(std::size_t, void* base)
{
    return base;
}

void* operator new[](std::size_t, void* base)
{
    return base;
}

void* operator new(std::size_t size)
{
    return echis::mm::AllocateMemory(size);
}

void operator delete(void* mem)
{
    return echis::mm::FreeMemory(mem);
}

void operator delete(void* mem, unsigned long)
{
    return echis::mm::FreeMemory(mem);
}

void* operator new[](std::size_t size)
{
    return echis::mm::AllocateMemory(size);
}

void operator delete[](void* mem)
{
    return echis::mm::FreeMemory(mem);
}

void operator delete[](void* mem, unsigned long)
{
    return echis::mm::FreeMemory(mem);
}