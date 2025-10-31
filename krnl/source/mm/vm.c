#include "mm/vm.h"
#include "mm/pm.h"
#include "mm/kheap.h"

#include <limine.h>

#include <string.h>

struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1,
    .response = NULL
};

struct limine_executable_address_request k_addr_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
    .revision = 1,
    .response = NULL
};

struct addrspace k_addrspace;
struct addrspace *curr_addrspace;

uint64_t hhdmTop = 0;

extern char _kernel_start[];
extern char _kernel_end[];

void *vm_phystovirt(physaddr_t addr)
{
    return (void *)(addr + hhdm_request.response->offset);
}

struct addrspace *vm_get_kaddrspace(void)
{
    return &k_addrspace;
}

struct addrspace *vm_get_addrspace(void)
{
    return curr_addrspace;
}

void vm_switch_to(struct addrspace *a)
{
    curr_addrspace = a;
    __asm__ volatile("mov %0, %%cr3" :: "r"(a->pml4));
}

static inline physaddr_t get_pt_address(physaddr_t phys)
{
    return (phys & ~0xFFFF000000000FFF);
}

struct addrspace make_addrspace(void)
{
    struct addrspace ret;
    ret.pml4 = pm_getpage();
    memset(vm_phystovirt(ret.pml4), 0, 0x1000);
    ret.pml4 = get_pt_address(ret.pml4);

    void *hhalf  = vm_phystovirt(ret.pml4 + 256 * sizeof(uint64_t));
    void *khhalf = vm_phystovirt(k_addrspace.pml4 + 256 * sizeof(uint64_t));

    memcpy(hhalf, khhalf, 256 * sizeof(uint64_t));

    ret.fl = kheap_alloc(sizeof(struct vm_allocator_node));
    ret.fl->base = 0x1000;
    ret.fl->nPages = NPAGES(0x00007FFFFFFFE000 - 0x1000);
    ret.fl->next = NULL;

    return ret;
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

    for (int i = 256; i < 512; ++i)
    {
        uint64_t *pml4 = vm_phystovirt(k_addrspace.pml4);
        pml4[i] = pm_getpage();
        memset((void *)pml4[i], 0, 0x1000);
        pml4[i] |= PT_PRESENT | PT_WRITE;
    }
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

        if (entry->base + entry->length > hhdmTop)
        {
            hhdmTop = entry->base + entry->length;
        }
    }
    hhdmTop = hhdmTop + hhdm_request.response->offset;
}

void vm_init(void)
{
    make_kernel_pml4();

    map_kernel();
    map_hhdm();

    vm_switch_to(&k_addrspace);
}

void vm_allocator_init(void)
{
    struct vm_allocator_node *node = kheap_alloc(sizeof(struct vm_allocator_node));
    node->base = hhdmTop;
    uint64_t size = (uint64_t)_kernel_start - hhdmTop;
    node->nPages = NPAGES(size);
    node->next = NULL;
    k_addrspace.fl = node;
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


void *vm_getpage(struct addrspace *a)
{
    return vm_getpages(a, 1);
}

static inline void fl_remove(struct addrspace *a, struct vm_allocator_node *curr, struct vm_allocator_node *prev)
{
    if (prev) prev->next = curr->next;
    else a->fl = curr->next;

    curr->next = NULL;
}

void *vm_getpages(struct addrspace *a, uint32_t count)
{
    if (!count) return NULL;
    if (!a) a = &k_addrspace;
    uint16_t flags = PT_PRESENT | PT_WRITE;
    if (a != &k_addrspace) flags |= PT_USER;

    struct vm_allocator_node *prev = NULL;
    struct vm_allocator_node *curr = a->fl;
    while (curr)
    {
        if (curr->nPages >= count) break;
        prev = curr;
        curr = curr->next;
    }
    if (!curr) return NULL;

    uint64_t base = curr->base;
    if (curr->nPages == count)
        fl_remove(a, curr, prev);
    else
    {
        curr->nPages -= count;
        curr->base += count * 0x1000;
    }

    // todo: lazy mapping
    for (uint32_t i = 0; i < count * 0x1000; i += 0x1000)
    {
        vm_map_page(
            a,
            pm_getpage(),
            base + i,
            flags
        );
    }

    // todo: add allocation to list of active VMAs
    return (void *)base;
}
