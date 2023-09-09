#include <mm/Pmm.h>

#include <string.h>

#include <limine.h>

#include <cstddef>
#include <new>

namespace PMM
{
    limine_hhdm_request hhdmRequest = {
        .id = LIMINE_HHDM_REQUEST,
        .revision = 1,
        .response = nullptr,
    };

    uint32_t NPAGES(uint64_t n)
    {
        return (n + PAGE_SIZE - 1) / PAGE_SIZE;
    }

    uint64_t VirtToPhys(uint64_t addr)
    {
        return addr - hhdmRequest.response->offset;
    }
    uint64_t PhysToVirt(uint64_t addr)
    {
        return addr + hhdmRequest.response->offset;
    }
    
    struct MemoryRegion
    {
        MemoryRegion(uint64_t size)
            :bitmap(nullptr), lastPage(nullptr), next(nullptr), totalSize(size) {}

        uint8_t* bitmap;
        uint8_t* base;
        uint8_t* lastPage;
        MemoryRegion* next;
        uint64_t totalSize;
        uint64_t totalFree;
    };

    void BmpMarkUsed(MemoryRegion* region, void* addr, uint32_t npages)
    {
        uint64_t address = (uint8_t*)addr - region->base;

        for(uint64_t i = address; i < address + (npages * PAGE_SIZE); i += PAGE_SIZE)
            region->bitmap[i / (PAGE_SIZE * 8)] &= ~(1 << ((i / PAGE_SIZE) % 8));
    }

    bool BmpTest(MemoryRegion* region, void* addr, uint32_t npages)
    {
        bool result = true;
        uint64_t address = (uint8_t*)addr - region->base;

        for(uint64_t i = address; i < address + (npages * PAGE_SIZE); i += PAGE_SIZE)
        {
            result = region->bitmap[i / (PAGE_SIZE * 8)] & (1 << ((i / PAGE_SIZE) % 8));
            if (!result)
                break;
        }

        return result;
    }

    static volatile limine_memmap_request memmapRequest = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 1,
        .response = nullptr,
    };

    static MemoryRegion* regions = nullptr;
    limine_memmap_response* MemMap;

    void Init()
    {
        MemMap = memmapRequest.response;
        for(size_t i = 0; i < memmapRequest.response->entry_count; i++)
        {
            limine_memmap_entry* entry = memmapRequest.response->entries[i];

            if(entry->base + entry->length < 0x100000)
                continue;

            if(entry->type == LIMINE_MEMMAP_USABLE)
            {
                MemoryRegion* region = new((char*)PhysToVirt(entry->base)) MemoryRegion(entry->length);
                uint64_t bitmapSize = region->totalSize / (PAGE_SIZE * 8);

                region->bitmap = new((char*)PhysToVirt(entry->base + sizeof(MemoryRegion))) uint8_t[bitmapSize];

                region->base = (uint8_t*)(((entry->base + (bitmapSize) + sizeof(MemoryRegion)) & ~0xFFF) + 0x1000);
                region->totalSize -= NPAGES(entry->base + (bitmapSize) + sizeof(MemoryRegion));
                region->totalFree = region->totalSize;

                memset((void*)region->bitmap, 0xFF, bitmapSize);

                if(!regions)
                    regions = region;
                else
                {
                    region->next = regions;
                    regions = region;
                }
            }
        }
    }

    PhysicalAddress GetPage()
    {
        return GetPages(1);
    }

    void FreePage(void* address)
    {
        FreePages(address, 1);
    }
    
    bool Alloc(MemoryRegion* region, void* address, uint32_t npages)
    {
        if(!BmpTest(region, address, npages))
            return false;
        
        BmpMarkUsed(region, address, npages);
        region->totalFree -= npages * PAGE_SIZE;
        region->lastPage = (uint8_t*)address;
        return true;
    }

    PhysicalAddress GetPages(uint32_t npages)
    {
        if(npages == 0)
            return 0;
        
        MemoryRegion* region = regions;
        while(region != nullptr)
        {
            if(region->lastPage != nullptr)
            {
                for(uint8_t* addr = region->lastPage; addr < region->base + region->totalSize; addr += PAGE_SIZE)
                {
                    if(Alloc(region, addr, npages))
                        return (PhysicalAddress)addr;
                }
            }
            for(uint8_t* addr = region->base; addr < region->base + region->totalSize; addr += PAGE_SIZE)
            {
                if(Alloc(region, addr, npages))
                    return (PhysicalAddress)addr;
            }
            region = region->next;
        }
        return 0;
    }

    void FreePages(void* address, uint32_t npages)
    {
        if(address == nullptr || npages == 0)
            return;

        MemoryRegion* region = regions;
        while(region != nullptr)
        {
            if(address >= region->base && address <= region->base + region->totalSize)
            {
                for(uint64_t i = (uint64_t)address; i < (uint64_t)address + npages * PAGE_SIZE; i += PAGE_SIZE)
                {
                    if(!BmpTest(region, (void*)i, 1))
                        region->totalFree += npages * PAGE_SIZE;

                    uint64_t addr = i - (uint64_t)region->base;
                    region->bitmap[addr / (PAGE_SIZE / 8)] |= 1 << ((addr / PAGE_SIZE) % 8);
                }
                return;
            }
            region = region->next;
        }
    }
}