#include <mm/Vmm.h>
#include <mm/Pmm.h>

#include <cpu/interrupt/Exception.h>

#include <algorithm.h>

namespace vm
{
    bool OnPageFault(InterruptFrame* frame)
    {
        uint64_t address = frame->controlRegisters.cr2;

        uint16_t flags = paging::GetPage(paging::AddressSpace::Current(), address) & 0xFFF;


        if (flags & paging::flags::demand)
        {
            flags &= ~paging::flags::demand; // No need to be demand paged anymore
            flags |=  paging::flags::present;
            paging::MapPage(paging::AddressSpace::Current(), PMM::GetPage(), address, flags);
            return false;
        }
        return true;
    }

    void Init()
    {
        exception::subscribe(exception::vectors::pageFault, OnPageFault);
    }


    void MapDemandPaged(paging::AddressSpace* addrspace, uint64_t address, uint32_t npages, uint16_t flags)
    {
        flags &= ~paging::flags::present;
        flags |=  paging::flags::demand;

        paging::MapPages(addrspace, 0, address, flags, npages);
    }

    void* GetPages(paging::AddressSpace* addrspace, uint32_t npages, uint16_t flags)
    {
        if (npages == 0)
        {
            return nullptr;
        }

        auto it = vpr::find_if(addrspace->nodes.begin(), addrspace->nodes.end(), [npages](const paging::VMNode& node){
            return (node.npages >= npages);
        });

        if (it == addrspace->nodes.end())
        {
            return nullptr; // TODO: Swap out memory/cause error
        }

        if (it->npages == npages)
        {
            void* base = it->base;
            addrspace->nodes.erase(it);

            MapDemandPaged(addrspace, (uint64_t)base, npages, flags);
            return base;
        }

        void* base = it->base;
        
        it->npages = it->npages - npages; // Take away allocated pages
        it->base   = (char*)it->base + npages * PMM::PAGE_SIZE;

        MapDemandPaged(addrspace, (uint64_t)base, npages, flags);
        return base;
    }
}