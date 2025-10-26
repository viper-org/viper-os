#include <new>


void* operator new(unsigned long, void* base)
{
    return base;
}

void* operator new[](unsigned long, void* base)
{
    return base;
}