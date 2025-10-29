#include "driver/ldr/loader.h"

#include "mm/vm.h"

#include <limine.h>

#include <string.h>

static volatile struct limine_kernel_file_request KernelFileRequest = {
    .id = LIMINE_KERNEL_FILE_REQUEST,
    .revision = 1,
    .response = NULL
};

typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int32_t  Elf64_Sword;
typedef int64_t  Elf64_Sxword;

typedef struct
{
    uint8_t	   e_ident[16];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off  e_phoff;
    Elf64_Off  e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct
{
    Elf64_Word	p_type;
    Elf64_Word	p_flags;
    Elf64_Off	p_offset;
    Elf64_Addr	p_vaddr;
    Elf64_Addr	p_paddr;
    Elf64_Xword	p_filesz;
    Elf64_Xword	p_memsz;
    Elf64_Xword	p_align;
} Elf64_Phdr;

typedef struct
{
    Elf64_Word sh_name;
    Elf64_Word sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off  sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word sh_link;
    Elf64_Word sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
} Elf64_Shdr;

typedef struct
{
    Elf64_Word	  st_name;
    unsigned char st_info;
    unsigned char st_other;
    Elf64_Half st_shndx;
    Elf64_Addr	  st_value;
    Elf64_Xword	  st_size;
} Elf64_Sym;

#define PT_LOAD 1
#define PF_DHDR (1 << 3)

static Elf64_Ehdr *kernelEhdr;
static Elf64_Shdr *symtab = NULL;
static char *strtab = NULL;

static inline Elf64_Shdr *section(Elf64_Ehdr *ehdr, int n)
{
    Elf64_Shdr *shdrStart = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
    return shdrStart + n;
}

static inline char *getShstrtab(Elf64_Ehdr *ehdr)
{
    return (char *)(ehdr) + section(ehdr, ehdr->e_shstrndx)->sh_offset;
}

void ldr_init(void)
{
    kernelEhdr = (Elf64_Ehdr *)(KernelFileRequest.response->kernel_file->address);
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
    void *vaddr_base = vm_getpages(NULL, size);

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
    header->read  = (ssize_t (*)(void *,size_t))((uint64_t)header->read + (uint64_t)vaddr_base);
    header->write = (ssize_t (*)(const void *,size_t))((uint64_t)header->write + (uint64_t)vaddr_base);
    header->ioctl = (int (*)(unsigned long, char *))((uint64_t)header->ioctl + (uint64_t)vaddr_base);

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
