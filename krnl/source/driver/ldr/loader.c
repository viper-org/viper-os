#include "driver/ldr/loader.h"

#include "mm/vm.h"

#include <limine.h>

#include <string.h>
#include <elf.h>

static volatile struct limine_executable_file_request KernelFileRequest = {
    .id = LIMINE_EXECUTABLE_FILE_REQUEST_ID,
    .revision = 1,
    .response = NULL
};

static Elf64_Ehdr *kernelEhdr;
static Elf64_Shdr *symtab = NULL;
static char *strtab = NULL;

void ldr_init(void)
{
    kernelEhdr = (Elf64_Ehdr *)(KernelFileRequest.response->executable_file->address);
    Elf64_Shdr* shdrStart = (Elf64_Shdr *)((uint64_t)kernelEhdr + kernelEhdr->e_shoff);
    char* shstrtab = getShstrtab(kernelEhdr);

    for (int i = 0; i < kernelEhdr->e_shnum; ++i)
    {
        if (!strcmp(shstrtab + shdrStart[i].sh_name, ".symtab"))
        {
            symtab = &shdrStart[i];
        }
        if (!strcmp(shstrtab + shdrStart[i].sh_name, ".strtab"))
        {
            strtab = (char*)kernelEhdr + shdrStart[i].sh_offset;
        }
    }
}

void *kernel_function_getter(const char* name);

struct driver ldr_load(void *addr)
{
    char *file = addr;
    Elf64_Ehdr *ehdr = addr;
    Elf64_Phdr *phdr = (Elf64_Phdr *)(file + ehdr->e_phoff);

    uint32_t size = 0;
    Elf64_Phdr *hdr_phdr = NULL;
    for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
    {
        if (phdr->p_type != PT_LOAD) continue;
        if (phdr->p_flags & PF_DHDR) hdr_phdr = phdr;
        size += phdr->p_memsz;
    }
    size = NPAGES(size);
    void *vaddr_base = vm_getpages(NULL, size, 0, NULL);

    phdr = (Elf64_Phdr *)(file + ehdr->e_phoff);
    for (uint32_t i = 0; i < ehdr->e_phnum; ++i, ++phdr)
    {
        if (phdr->p_type != PT_LOAD)
        {
            continue;
        }

        void *vaddr = (char *)vaddr_base + phdr->p_vaddr;

        char *start = file + phdr->p_offset;
        memcpy(vaddr, start, phdr->p_filesz);
        char *bss = (char *)vaddr + phdr->p_filesz;
        memset(bss, 0, phdr->p_memsz - phdr->p_filesz);
    }

    char *hdr_loc = (char *)vaddr_base + hdr_phdr->p_vaddr;
    struct driver_header *header = (struct driver_header *)hdr_loc;
    header->kernel_func_getter = &kernel_function_getter;

    // "relocate" header fields
    header->name  += (uint64_t)vaddr_base;
    header->read  = (ssize_t (*)(void *,size_t *,size_t))((uint64_t)header->read + (uint64_t)vaddr_base);
    header->write = (ssize_t (*)(const void *,size_t,size_t))((uint64_t)header->write + (uint64_t)vaddr_base);
    header->stat  = (int (*)(struct stat *))((uint64_t)header->stat + (uint64_t)vaddr_base);
    header->ioctl = (int (*)(unsigned long, char *))((uint64_t)header->ioctl + (uint64_t)vaddr_base);
    header->mmap = (void (*)(void *, void *, size_t))((uint64_t)header->mmap + (uint64_t)vaddr_base);

    return (struct driver) {
        .header = header,
        .init = (void(*)(void))(ehdr->e_entry + (uint64_t)vaddr_base),
    };
}

void *kernel_function_getter(const char *name)
{
    Elf64_Sym *symbol = (Elf64_Sym *)((char *)kernelEhdr + symtab->sh_offset);

    for (uint64_t i = 0; i < symtab->sh_size / sizeof(Elf64_Sym); ++i)
    {
        if (!strcmp(strtab + symbol[i].st_name, name))
        {
            return (void *)(symbol[i].st_value);
        }
    }
    return (void *)0xCA11CA11CA11CA11;
}
