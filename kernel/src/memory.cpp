#include <memory.h>
#include <mm/Heap.h>

void* operator new(size_t bytes)
{
    return mm::Alloc(bytes);
}

void operator delete(void* address)
{
    mm::Free(address);
}