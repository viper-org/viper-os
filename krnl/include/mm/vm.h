#ifndef VIPEROS_MM_VM
#define VIPEROS_MM_VM 1

#include <stdint.h>

typedef unsigned long long physaddr_t;

void *vm_phystovirt(physaddr_t);

enum pt_flags
{
    PT_PRESENT = 1 << 0,
    PT_WRITE   = 1 << 1,
    PT_USER    = 1 << 2,
    PT_PWT     = 1 << 3,
};

#define NPAGES(n) ((n + 0x1000 - 1) / 0x1000)

struct vm_allocator_node
{
    uint64_t base;
    uint64_t nPages;
    struct vm_allocator_node *next;
};

enum vma_flags
{
    VMA_NO = 1 << 0, // do npt make a VMA, should only be used for kernel heap allocations
    VMA_FILE_MAPPED = 1 << 1,
};

struct vma
{
    uint64_t base;
    uint64_t npages;

    uint16_t flags;
    struct vnode *mapping;

    struct vma *next;
};

struct addrspace
{
    physaddr_t pml4;

    struct vm_allocator_node *fl;
    struct vma *vma_ll;
};

struct addrspace *vm_get_kaddrspace(void);
struct addrspace *vm_get_addrspace(void);
void vm_switch_to(struct addrspace *a);
void vm_set_curr(struct addrspace *a); // set vm_get_addrspace() but don't switch cr3
struct addrspace make_addrspace(void);
void free_addrspace(struct addrspace *a);

void vm_init(void);
void vm_allocator_init(void);

void vm_map_page(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags);
void vm_map_pages(struct addrspace *a, physaddr_t phys, uint64_t virt, uint16_t flags, uint32_t count);
uint64_t vm_get_phys(struct addrspace *a, uint64_t virt);


void* vm_getpage(struct addrspace *);
void* vm_getpages(struct addrspace *, uint32_t count, uint16_t flags, struct vnode *map);

void vm_freepages(struct addrspace *, void *pages, uint32_t count);

#endif // VIPEROS_MM_VM
