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
 * Generic function to get the current
 * address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * static atheris::vm::AddressSpace* atheris::vm::AddressSpace::Current()
 */

/*
 * Enum providing flags for page mappings
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * enum atheris::vm::flags::flags : uint16_t
 * {
 *     present,
 *     write   
 * }
 */

/*
 * Structure describing an address space
 *
 * Must be provided in atheris/private/mm/vm.h
 *
 * struct AddressSpace
 */

#include <atheris/private/mm/vm.h>

#endif // VIPEROS_ATHERIS_MM_VM_H