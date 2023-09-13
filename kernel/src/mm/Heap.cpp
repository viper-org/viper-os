#include <mm/Heap.h>
#include <mm/Pmm.h>
#include <mm/Paging.h>

#include <new>

struct Header
{
    size_t size;
    Header* next;
};

Header* freeList;

namespace mm
{
    void Init()
    {
        constexpr int HEAP_SIZE = 32;

        constexpr uint64_t maxMemory = 0xffffffffffffffff;
        uint64_t virtualAddress = maxMemory - (HEAP_SIZE + 1) * PMM::PAGE_SIZE;

        PMM::PhysicalAddress physicalAddress = PMM::GetPages(HEAP_SIZE);
        Paging::MapPages(nullptr, physicalAddress, virtualAddress, 0x3, HEAP_SIZE);

        void* fl = (void*)virtualAddress;
        freeList = new(fl) Header;
        freeList->size = HEAP_SIZE * PMM::PAGE_SIZE;
        freeList->next = nullptr;
    }

    void* Alloc(size_t count)
    {
        if(!count)
            return nullptr;

        
        count += sizeof(Header);
        Header* current = freeList;
        Header* previous = nullptr;

        const auto removeFromFreeList = [&current, &previous]() {
            if(previous)
                previous->next = current->next;
            else
                freeList = current->next;
        };
        while(current)
        {
            if(current->size >= count)
            {
                if(current->size == count)
                {
                    removeFromFreeList();
                    
                    return current + 1;
                }
                size_t newSize = current->size - count;
                Header* newHeader = new((char*)current + count + sizeof(Header)) Header;
                newHeader->size = newSize;
                removeFromFreeList();
                
                newHeader->next = freeList;
                freeList = newHeader;

                current->size = count;
                return current + 1;
            }
            previous = current;
            current = current->next;
        }
        return nullptr;
    }

    void Free(void* addr)
    {
        if(addr == nullptr)
            return;
        
        Header* header = (Header*)addr - 1;
        header->next = freeList;
        freeList = header;
    }
}