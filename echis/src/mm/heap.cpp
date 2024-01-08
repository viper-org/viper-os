#include <mm/heap.h>
#include <mm/pmm.h>

#include <util/math.h>

#include <std/thread/mutex.h>

#include <atheris/mm/vm.h>

#include <stdint.h>

extern char _kernel_end[];

namespace echis
{
    namespace mm
    {
        struct Header
        {
            size_t size;
            Header* next;
        };
        Header* freeList;
        vpr::mutex heapMutex;

        constexpr uint32_t HeapPages = 32;

        void Init()
        {
            uint64_t firstVirtualPage = util::AlignUp(reinterpret_cast<uint64_t>(_kernel_end), pmm::GetPageSize());
            for (uint32_t i = 0; i < HeapPages; ++i)
            {
                atheris::vm::MapPage(nullptr,
                                     pmm::GetPage(),
                                     firstVirtualPage + (i * pmm::GetPageSize()),
                                     atheris::vm::flags::present | atheris::vm::flags::write);
            }

            freeList       = reinterpret_cast<Header*>(firstVirtualPage);
            freeList->size = HeapPages * pmm::GetPageSize();
            freeList->next = nullptr;
        }

        void* Alloc(size_t count)
        {
            vpr::lock_guard<vpr::mutex> lock(heapMutex);
            if (!count)
            {
                return nullptr;
            }

            count += sizeof(Header);
            Header* current  = freeList;
            Header* previous = nullptr;

            const auto removeFromFreeList = [](Header* current, Header* previous) {
                if (previous)
                {
                    previous->next = current->next;
                }
                else
                {
                    freeList = current->next;
                }
            };

            while(current)
            {
                if (current->size >= count)
                {
                    if (current->size == count)
                    {
                        removeFromFreeList(current, previous);

                        return current + 1;
                    }

                    size_t newSize = current->size - count;
                    char* newHeaderLocation = reinterpret_cast<char*>(current + count);
                    Header* newHeader = reinterpret_cast<Header*>(newHeaderLocation);
                    newHeader->size = newSize;

                    removeFromFreeList(current, previous);

                    newHeader->next = freeList;
                    freeList = newHeader;

                    current->size -= count;
                    return current + 1;
                }
                previous = current;
                current = current->next;
            }
            
            return nullptr; // TODO: OOM - allocate more
        }

        void Free(void* mem)
        {
            vpr::lock_guard<vpr::mutex> lock(heapMutex);
            if (!mem)
            {
                return;
            }

            Header* header = reinterpret_cast<Header*>(mem);
            header->next = freeList;
            freeList = header;
        }
    }
}

void* operator new(size_t size)
{
    return echis::mm::Alloc(size);
}

void operator delete(void* mem)
{
    return echis::mm::Free(mem);
}

void* operator new[](size_t size)
{
    return echis::mm::Alloc(size);
}

void operator delete[](void* mem)
{
    return echis::mm::Free(mem);
}