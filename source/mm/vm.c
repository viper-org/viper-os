#include "mm/vm.h"
#include "mm/pm.h"

#include <limine.h>

#include <string.h>

struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1,
    .response = NULL
};

struct limine_kernel_address_request k_addr_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 1,
    .response = NULL
};

struct addrspace k_addrspace;

extern char _kernel_start[];
extern char _kernel_end[];

void *vm_phystovirt(physaddr_t addr)
{
    return (void *)(addr + hhdm_request.response->offset);
}

void vm_switch_to(struct addrspace *a)
{
    __asm__ volatile("mov %0, %%cr3" :: "r"(a->pml4));
}

static inline physaddr_t get_pt_address(physaddr_t phys)
{
    return (phys & ~0xFFFF000000000FFF);
}

static inline physaddr_t get_page_level(void)
{
    physaddr_t phys = pm_getpage();
    void* virt = vm_phystovirt(phys);
    memset(virt, 0, 0x1000);
    return phys;
}

static inline void make_kernel_pml4(void)
{
    k_addrspace.pml4 = get_page_level();
}

static inline void map_kernel(void)
{
    physaddr_t pBase = k_addr_request.response->physical_base;
    uint64_t   vBase = k_addr_request.response->virtual_base;

    vm_map_pages(
        &k_addrspace,
        pBase,
        vBase,
        PT_PRESENT | PT_WRITE,
        NPAGES(_kernel_end - _kernel_start)
    );
}

static inline void map_hhdm(void)
{
    uint64_t hhdmBase = hhdm_request.response->offset;
    
    // Map first 4GiB
    vm_map_pages(
        &k_addrspace,
        0,
        hhdmBase,
        PT_PRESENT | PT_WRITE,
        NPAGES(0x400000)
    );

    for (uint64_t i = 0; i < pm_get_memmap()->entry_count; ++i)
    {
        struct limine_memmap_entry *entry = pm_get_memmap()->entries[i];

        vm_map_pages(
            &k_addrspace,
            entry->base,
            entry->base + hhdmBase,
            PT_PRESENT | PT_WRITE,
            NPAGES(entry->base)
        );
    }
}

void vm_init(void)
{
    make_kernel_pml4();

    map_kernel();
    map_hhdm();

    vm_switch_to(&k_addrspace);
}

void vm_map_page(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags)
{
    if (!a)
    {
        a = &k_addrspace;
    }

    phys  &= ~0xFFF;
    virt  &= ~0xFFF;
    flags &=  0xFFF;

    uint64_t shift = 48;
    uint64_t* pt = vm_phystovirt(get_pt_address(a->pml4));
    uint64_t index;
    for (int i = 0; i < 4; ++i)
    {
        shift -= 9;
        index = (virt >> shift) & 0x1FF;
        if (i == 3) break;

        if (!(pt[index] & PT_PRESENT))
        {
            pt[index] = get_page_level();
            pt[index] |= flags | PT_PRESENT;
        }

        pt = vm_phystovirt(get_pt_address(pt[index]));
    }

    pt[index] = phys | flags;
}

void vm_map_pages(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags, uint32_t count)
{
    for (uint32_t i = 0; i < count * 0x1000; i += 0x1000)
    {
        vm_map_page(
            a,
            phys + i,
            virt + i,
            flags
        );
    }
}
