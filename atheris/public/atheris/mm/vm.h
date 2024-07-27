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
 */

#include <atheris/private/mm/vm.h>

#endif // VIPEROS_ATHERIS_PUBLIC_MM_VM_H