#include <mm/heap.h>
#include <mm/physical.h>

#include <util/math.h>

#include <atheris/mm/vm.h>

#include <stdint.h>

extern "C" char _kernel_end[];

namespace echis
{
    namespace mm
    {
        struct Header
        {
            std::size_t size;
            Header* next;
        };
        Header* freeList;
        
        constexpr uint32_t HeapPages = 32;

        void Init()
        {
            uint64_t firstVirtualPage = util::AlignUp(reinterpret_cast<uint64_t>(_kernel_end), physical::GetPageSize());
            for (uint32_t i = 0; i < HeapPages; ++i)
            {
                atheris::vm::MapPage(nullptr,
                                     physical::GetPage(),
                                     firstVirtualPage + (i * physical::GetPageSize()),
                                     atheris::vm::flags::present | atheris::vm::flags::write);
            }

            freeList       = reinterpret_cast<Header*>(firstVirtualPage);
            freeList->size = HeapPages * physical::GetPageSize();
            freeList->next = nullptr;
        }

        void* AllocateMemory(std::size_t count)
        {
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
                current->next = nullptr;
            };

            while(current)
            {
                if (current->size >= count)
                {
                    if (current->size == count || ((current->size - count) < sizeof(Header) + 1))
                    {
                        removeFromFreeList(current, previous);
                        return current + 1;
                    }

                    std::size_t newSize = current->size - count;
                    char* newHeaderLocation = reinterpret_cast<char*>(current) + count;
                    Header* newHeader = reinterpret_cast<Header*>(newHeaderLocation);
                    newHeader->size = newSize;

                    removeFromFreeList(current, previous);

                    newHeader->next = freeList;
                    freeList = newHeader;

                    current->size = count;
                    return current + 1;
                }
                previous = current;
                current = current->next;
            }
            
            return nullptr; // TODO: OOM - allocate more
        }

        void FreeMemory(void* mem)
        {
            if (!mem)
            {
                return;
            }

            Header* header = reinterpret_cast<Header*>(mem) - 1;
            header->next = freeList;
            freeList = header;
        }
    }
}