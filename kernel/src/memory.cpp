#include <memory.h>
#include <mm/Heap.h>

void* NewObject(size_t bytes)
{
    return mm::Alloc(bytes);
}

void* operator new(size_t bytes)
{
    return NewObject(bytes);
}

void* operator new[](size_t bytes)
{
    return NewObject(bytes);
}


void DeleteObject(void* address)
{
    mm::Free(address);
}

void operator delete(void* address)
{
    DeleteObject(address);
}
void operator delete(void* address, size_t)
{
    DeleteObject(address);
}