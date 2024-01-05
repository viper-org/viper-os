#include <mm/util.h>

#include <limine.h>

namespace x64
{
    limine_hhdm_request hhdmRequest = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 1,
        .response = nullptr,
    };

    uint32_t NPAGES(uint64_t n)
    {
        return (n + 0x1000 - 1) / 0x1000;
    }

    uint64_t VirtToPhys(uint64_t addr)
    {
        return addr - hhdmRequest.response->offset;
    }
    uint64_t PhysToVirt(uint64_t addr)
    {
        return addr + hhdmRequest.response->offset;
    }
    
    uint64_t ValidatePageTable(uint64_t pageTable)
    {
        return (pageTable & ~0xFFFF000000000FFF);
    }

    uint64_t GetHHDMOffset()
    {
        return hhdmRequest.response->offset;
    }
}