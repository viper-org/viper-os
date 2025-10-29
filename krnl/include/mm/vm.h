#ifndef VIPEROS_MM_VM
#define VIPEROS_MM_VM 1

#include <stdint.h>

typedef unsigned long long physaddr_t;

void *vm_phystovirt(physaddr_t);

enum pt_flags
{
    PT_PRESENT = 1 << 0,
    PT_WRITE   = 1 << 1
};

#define NPAGES(n) ((n + 0x1000 - 1) / 0x1000)

struct vm_allocator_node
{
    uint64_t base;
    uint64_t nPages;
    struct vm_allocator_node *next;
};

struct addrspace
{
    physaddr_t pml4;

    struct vm_allocator_node *fl;
};

struct addrspace *vm_get_kaddrspace(void);
void vm_switch_to(struct addrspace *a);
struct addrspace make_addrspace(void);

void vm_init(void);
void vm_allocator_init(void);

void vm_map_page(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags);
void vm_map_pages(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags, uint32_t count);


void* vm_getpage(struct addrspace *);
void* vm_getpages(struct addrspace *, uint32_t count);

#endif // VIPEROS_MM_VM
