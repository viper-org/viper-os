#include <memory.h>
#include <mm/Heap.h>

void* operator new(size_t bytes)
{
    return mm::Alloc(bytes);
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