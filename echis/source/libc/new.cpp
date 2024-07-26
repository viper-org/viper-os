#include <new>

void* operator new(std::size_t, void* base)
{
    return base;
}

void* operator new[](std::size_t, void* base)
{
    return base;
}