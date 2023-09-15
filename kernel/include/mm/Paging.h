#ifndef MEM_PAGING_HPP
#define MEM_PAGING_HPP

#include <container/list.h>

#include <stdint.h>

namespace paging
{
    struct VMNode
    {
        uint32_t npages;
        void* base;
    };

    struct AddressSpace
    {
        void switchTo();

        static AddressSpace  Create();
        static AddressSpace* Current();

        uint64_t* pml4;
        vpr::list<VMNode> nodes;
    };

    void Init();

    void MapPage(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
    void MapPages(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

    uint64_t GetPage(AddressSpace* addrspace, uint64_t VirtAddr);

    AddressSpace* KernelAddrSpace();

    namespace flags
    {
        enum
        {
            present = 1 << 0,
            write   = 1 << 1,

            demand  = 1 << 10,
        };
    }
}

#endif