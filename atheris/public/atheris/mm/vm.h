#ifndef VIPEROS_ATHERIS_PUBLIC_MM_VM_H
#define VIPEROS_ATHERIS_PUBLIC_MM_VM_H 1


/*
 * Generic function to get the virtual address of
 * a provided physical address
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void* atheris::vm::GetVirtualAddress(atheris::pm::physaddr physAddress)
 */

/*
 * Generic function to initialize page mapper
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::Init()
 */

/*
 * Generic function that initializes kernel virtual
 * memory allocator free list(adding all allowed
 * addresses)
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::InitAllocator(std::list<echis::vm::VMAllocNode>& freeList)
 */

/*
 * Generic function to map a single page with
 * specified flags
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::MapPage(atheris::vm::AddressSpace* addressSpace, atheris::pm::physaddr phys, uint64_t virt, uint16_t flags)
 */

/*
 * Generic function to map a group of consecutive
 * pages with specified flags
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::MapPage(atheris::vm::AddressSpace* addressSpace, atheris::pm::physaddr phys, uint64_t virt, uint16_t flags, uint32_t pageCount)
 */

/*
 * Function that returns the flags that a page
 * was mapped with(or zero if none)
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * std::uint16_t atheris::vm::GetFlags(atheris::vm::AddressSpace* addressSpace, std::uint64_t address)
 */

/*
 * Enum providing the allowed flags for page
 * mappings
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * enum atheris::vm::flags::flags : uint16_t
 * {
 *      present,
 *      write,
 * }
 */

/*
 * Structure describing an address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * struct atheris::vm::AddressSpace
 * {
 *      std::list<echis::vm::VMAllocNode> freeNodes;
 *      std::list<echis::vm::VMAllocatedRegion> allocatedRegions;
 * }
 */

/*
 * Function that returns the active address space
 * at the time of calling
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * atheris::vm::AddressSpace* atheris::vm::AddressSpace::Active()
 */

/*
 * Function that returns the initial kernel
 * address space used for system setup
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * atheris::vm::AddressSpace* atheris::vm::AddressSpace::Kernel()
 */

#include <atheris/private/mm/vm.h>

#endif // VIPEROS_ATHERIS_PUBLIC_MM_VM_H