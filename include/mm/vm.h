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

struct addrspace
{
    physaddr_t pml4;
};

void vm_switch_to(struct addrspace *a);

void vm_init(void);

void vm_map_page(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags);
void vm_map_pages(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags, uint32_t count);

#endif // VIPEROS_MM_VM
