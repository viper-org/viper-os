#ifndef VIPEROS_LDR_ELF
#define VIPEROS_LDR_ELF 1

#include "mm/vm.h"

#include <stdint.h>

struct elf_exec
{
    uint64_t entry;
};

struct elf_exec load_elf(void *file, struct addrspace *a);

#endif // VIPEROS_LDR_ELF
