#include "mm/pm.h"
#include "mm/vm.h"

#include <limine.h>

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

struct mem_region
{
    uint8_t *bmpStart;
    uint8_t *bmpEnd;

    physaddr_t base;
    physaddr_t lastAllocation;

    struct mem_region *next;

    uint32_t totalSize;
    uint32_t totalFree;
};

struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 1,
    .response = NULL
};

int align_up(int v, int align)
{
    return (v & ~(align - 1)) + align;
}

struct mem_region* fl;

void pm_addregion(physaddr_t base, size_t length)
{
    void *vbase = vm_phystovirt(base);
    struct mem_region *region = vbase;
    region->lastAllocation = 0;
    region->next = NULL;
    region->totalSize = length;

    uint32_t bmpSize = (length - sizeof (struct mem_region)) / (0x1000 * 8);
    uint8_t *bmpStart = (uint8_t *)vbase + sizeof (struct mem_region);
    region->bmpStart = bmpStart;
    region->bmpEnd = bmpStart + bmpSize;

    region->base = align_up(base + sizeof (struct mem_region) + bmpSize, 0x1000);
    region->totalSize -= (region->base - base);
    region->totalFree = region->totalSize;

    memset(region->bmpStart, 0xFF, bmpSize);

    if (fl) region->next = fl;
    fl = region;
}

void pm_init(void)
{
    for (size_t i = 0; i < memmap_request.response->entry_count; ++i)
    {
        struct limine_memmap_entry* e = memmap_request.response->entries[i];

        if (e->type == LIMINE_MEMMAP_USABLE)
            pm_addregion(e->base, e->length);
    }
}

static inline bool bmp_test(struct mem_region *region, physaddr_t addr, uint32_t count)
{
    bool result = true;
    uint64_t idx = addr - region->base;

    uint64_t endIdx = (idx + (count * 0x1000 - 1)) / (0x1000 * 8);
    if (&region->bmpStart[endIdx] >= region->bmpEnd) return false;

    for (size_t i = idx; i < idx + (0x1000 * count); i += 0x1000)
    {
        result = region->bmpStart[i / (0x1000 * 8)] & (1 << ((i / 0x1000) % 8));
        if (!result) break;
    }

    return result;
}

static inline void bmp_set(struct mem_region *region, physaddr_t addr, uint32_t count)
{
    uint64_t idx = addr - region->base;

    for (size_t i = idx; i < idx + (count * 0x1000); i += 0x1000)
    {
        region->bmpStart[i / (0x1000 * 8)] &= ~(1 << ((i / 0x1000) % 8));
    }
}

bool try_alloc(struct mem_region *region, physaddr_t addr, uint32_t count)
{
    if (!bmp_test(region, addr, count)) return false;

    bmp_set(region, addr, count);
    region->totalFree -= count * 0x1000;
    region->lastAllocation = addr + count * 0x1000;
    return true;
}

physaddr_t check_region_from(struct mem_region *region, physaddr_t addr, uint32_t count)
{
    const physaddr_t end = region->base + region->totalSize;

    for (physaddr_t i = addr; i < end; i += 0x1000)
    {
        if (try_alloc(region, i, count)) return i;
    }

    return 0;
}

physaddr_t check_region(struct mem_region *region, uint32_t count)
{
    if (region->lastAllocation != 0)
    {
        physaddr_t addr = check_region_from(region, region->lastAllocation, count);
        if (addr) return addr;
    }

    physaddr_t addr = check_region_from(region, region->base, count);
    if (addr) return addr;

    return 0;
}

physaddr_t pm_getpage(void)
{
    return pm_getpages(1);
}

physaddr_t pm_getpages(int n)
{
    if (!n) return 0;

    struct mem_region *curr = fl;
    while (curr)
    {
        physaddr_t addr = check_region(curr, n);
        if (addr) return addr;

        curr = curr->next;
    }

    return 0;
}

void free_page_in(struct mem_region *region, physaddr_t page)
{
    if (!bmp_test(region, page, 1))
        region->totalFree += 0x1000;

    uint64_t idx = page - region->base;
    region->bmpStart[idx / (0x1000 * 8)] |= ((idx / 0x1000) % 8);
}

bool try_free_in(struct mem_region *region, physaddr_t pages, uint32_t count)
{
    physaddr_t pEnd = pages + (count * 0x1000);
    physaddr_t end = region->base + region->totalSize;

    if (pages >= region->base && pEnd <= end)
    {
        for (physaddr_t page = pages; page < pEnd; page += 0x1000)
            free_page_in(region, page);
        return true;
    }
    return false;
}

void pm_freepage(physaddr_t addr)
{
    pm_freepages(addr, 1);
}

void pm_freepages(physaddr_t addr, int n)
{
    struct mem_region *curr = fl;
    while (curr)
    {
        if (try_free_in(curr, addr, n)) return;

        curr = curr->next;
    }
}

struct limine_memmap_response *pm_get_memmap(void)
{
    return memmap_request.response;
}
