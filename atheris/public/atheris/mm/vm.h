#ifndef VIPEROS_ATHERIS_MM_VM_H
#define VIPEROS_ATHERIS_MM_VM_H 1

/*
 * Generic function to initialize VMM
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::Init()
 */

/*
 * Generic function to map a page with
 * specified flags
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::MapPage(atheris::vm::AddressSpace*, echis::pmm::physaddr, uint64_t, uint16_t)
 */

/*
 * Generic function to map a group of
 * consecutive pages with specified flags
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::MapPages(atheris::vm::AddressSpace*, echis::pmm::physaddr, uint64_t, uint16_t, uint32_t)
 */

/*
 * Generic function to return the virtual
 * address of a provided physical address
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void* atheris::vm::GetVirtualAddress(echis::pmm::physaddr)
 */

/*
 * Generic function to get the flags
 * that a virtual page has been mapped with
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * uint16_t atheris::vm::GetFlags(atheris::vm::AddressSpace*, uint64_t)
 */

/*
 * Generic function to get the current
 * address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * static atheris::vm::AddressSpace* atheris::vm::AddressSpace::Current()
 */

/*
 * Generic function to construct a new
 * address space with all of the kernel
 * mappings
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * static atheris::vm::AddressSpace atheris::vm::AddressSpace::Create()
 */

/*
 * Generic function to switch to an
 * address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * void atheris::vm::AddressSpace::switchTo()
 */

/*
 * Enum providing flags for page mappings
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * enum atheris::vm::flags::flags : uint16_t
 * {
 *     present,
 *     write,
 *     user,
 *     lazy
 * }
 */

/*
 * Structure describing a node in an
 * address space's free list for virtual
 * memory allocation
 *
 * Must be provided in atheris/private/mm.vm.h
 *
 * struct VMNode
 * {
 *     size_t   pages;
 *     uint64_t base;
 *     VMNode*  next;
 * }
 */

/*
 * Structure describing an address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * struct AddressSpace
 * {
 *     VMNode* freeList;
 * }
 */

#include <atheris/private/mm/vm.h>

#endif // VIPEROS_ATHERIS_MM_VM_H