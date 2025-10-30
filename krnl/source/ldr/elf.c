#include "ldr/elf.h"

#include "mm/vm.h"
#include "mm/pm.h"

#include "util.h"

#include <string.h>

struct Elf64_Ehdr
{
    unsigned char e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

#define PT_LOAD 1
struct Elf64_Phdr
{
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

struct elf_exec load_elf(void *addr, struct addrspace *a)
{
    struct addrspace *prev = vm_get_addrspace();
    vm_switch_to(a);

    char *file = addr;
    struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)addr;
    struct Elf64_Phdr *phdr = (struct Elf64_Phdr *)(file + ehdr->e_phoff);

    for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
    {
        if (phdr->p_type != PT_LOAD)
        {
            continue;
        }

        uint64_t vaddr = phdr->p_vaddr;
        uint32_t size = align_up(phdr->p_memsz, 0x1000);
        if ((vaddr & ~0xfff) != ((vaddr + phdr->p_memsz) & ~0xfff)) // If it crosses a page boundary
        {
            size += 1;
        }
        for (uint32_t j = 0; j < size; j += 0x1000)
        {
            vm_map_page(a,
                pm_getpage(),
                vaddr + j,
                PT_PRESENT | PT_WRITE | PT_USER);
        }
        char *fileOffset = file + phdr->p_offset;
        memcpy((void *)vaddr, fileOffset, phdr->p_filesz);
        char *bss = (char *)vaddr + phdr->p_filesz;
        memset(bss, 0, phdr->p_memsz - phdr->p_filesz);
        
        // todo: mark used in vm
    }

    vm_switch_to(prev);
    return (struct elf_exec) {
        .entry = ehdr->e_entry
    };
}
