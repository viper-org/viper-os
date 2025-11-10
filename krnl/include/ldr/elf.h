#ifndef VIPEROS_LDR_ELF
#define VIPEROS_LDR_ELF 1

#include "mm/vm.h"

#include <stdint.h>

struct elf_exec
{
    uint64_t entry;

    // auxvals
    uint64_t at_entry;
    uint64_t at_phdr;
    uint64_t at_phent;
    uint64_t at_phnum;
    uint64_t at_base1;
};

enum elf_auxval
{
    AT_ATSIZE=1,
    AT_ENTRY,
    AT_PHDR,
    AT_PHENT,
    AT_PHNUM,
    AT_BASE1
};

struct elf_exec load_elf(void *file, struct addrspace *a);

uint64_t push_argvc(int argc, char *argv, uint64_t ustack_top, size_t total_len);
void push_elf_auxvals(struct elf_exec *elf, uint64_t ustack_top);

#endif // VIPEROS_LDR_ELF
