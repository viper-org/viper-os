#include "ldr/elf.h"

#include "driver/dbg.h"
#include "mm/vm.h"
#include "mm/pm.h"

#include "util.h"

#include <string.h>
#include <elf.h>

struct elf_exec load_exec(char *file, struct addrspace *a);
struct elf_exec load_dyn(char *file, struct addrspace *a);

struct elf_exec load_elf(void *addr, struct addrspace *a)
{
    struct addrspace *prev = vm_get_addrspace();
    vm_switch_to(a);

    char *file = addr;
    struct Elf64_Ehdr *ehdr = addr;
    
    switch (ehdr->e_type)
    {
        case ET_EXEC:
        {
            struct elf_exec ret = load_exec(file, a);
            vm_switch_to(prev);
            return ret;
        }
        case ET_DYN:
        {
            struct elf_exec ret = load_dyn(file, a);
            vm_switch_to(prev);
            return ret;
        }
    }
}

struct elf_exec load_exec(char *file, struct addrspace *a)
{
    struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)file;
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

    return (struct elf_exec) {
        .entry = ehdr->e_entry
    };
}

struct elf_exec load_dyn(char *file, struct addrspace *a)
{
    struct Elf64_Ehdr *ehdr = (struct Elf64_Ehdr *)file;
    struct Elf64_Phdr *phdr = (struct Elf64_Phdr *)(file + ehdr->e_phoff);

    struct Elf64_Phdr *dynamic = NULL;
    uint32_t v_start = 0;
    uint32_t v_end = 0;
    for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
    {
        if (phdr->p_type == PT_LOAD)
        {
            if (phdr->p_vaddr < v_start) v_start = phdr->p_vaddr;
            if (phdr->p_vaddr + phdr->p_memsz > v_end) v_end = phdr->p_vaddr + phdr->p_memsz;
        }
        if (phdr->p_type == PT_DYNAMIC) dynamic = phdr;
    }

    void *vaddr_base = vm_getpages(a, NPAGES(v_end - v_start));

    phdr = (struct Elf64_Phdr *)(file + ehdr->e_phoff);
    for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
    {
        if (phdr->p_type != PT_LOAD)
        {
            continue;
        }

        void *vaddr = (char *)vaddr_base + phdr->p_vaddr;

        char *fileOffset = file + phdr->p_offset;
        memcpy((void *)vaddr, fileOffset, phdr->p_filesz);
        char *bss = (char *)vaddr + phdr->p_filesz;
        memset(bss, 0, phdr->p_memsz - phdr->p_filesz);
    }
    
    struct Elf64_Dyn *tag = file + dynamic->p_offset;
    uint64_t rela_addr = NULL;
    int relasz;
    while (tag->d_tag != DT_NULL)
    {
        switch (tag->d_tag)
        {
            case DT_RELA:
                rela_addr = tag->d_un.d_ptr;
                break;

            case DT_RELASZ:
                relasz = tag->d_un.d_val;
                break;

            default: break;
        }
        ++tag;
    }

    struct Elf64_Rela *rela = rela_addr + (char *)vaddr_base;
    while ((char *)rela < (char *)vaddr_base + rela_addr + relasz)
    {
        switch (ELF64_R_TYPE(rela->r_info))
        {
            case R_X86_64_RELATIVE:
            {
                char *addr = (char *)(vaddr_base) + rela->r_offset;
                *(uint64_t*)addr = (uint64_t)vaddr_base + rela->r_addend;
                break;
            }
            default:
                dbg_printf("Unkown ELF64_R_TYPE found: %d. Ignoring.\n\n", ELF64_R_TYPE(rela->r_info));
                break;
            // todo: others
        }
        ++rela;
    }

    return (struct elf_exec) {
        .entry = ehdr->e_entry + (uint64_t)vaddr_base
    };
}
