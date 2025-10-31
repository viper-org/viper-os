#ifndef VIPEROS_ELF_H
#define VIPEROS_ELF_H 1

#include <stdint.h>

typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int32_t  Elf64_Sword;
typedef int64_t  Elf64_Sxword;

#define ET_EXEC 2
#define ET_DYN 3
typedef struct Elf64_Ehdr
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

typedef struct Elf64_Phdr
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

typedef struct Elf64_Shdr
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

typedef struct Elf64_Sym
{
    Elf64_Word	  st_name;
    unsigned char st_info;
    unsigned char st_other;
    Elf64_Half st_shndx;
    Elf64_Addr	  st_value;
    Elf64_Xword	  st_size;
} Elf64_Sym;

struct Elf64_Dyn
{
    Elf64_Xword d_tag;
    union {
        Elf64_Xword d_val;
        Elf64_Addr d_ptr;
    } d_un;
};
#define DT_NULL 0
#define DT_RELA 7
#define DT_RELASZ 8
#define DT_RELAENT 9

struct Elf64_Rela
{
    Elf64_Addr r_offset;
    Elf64_Xword r_info;
    Elf64_Sxword r_addend;
};

#define ELF64_R_TYPE(info) ((Elf64_Word)info)

#define R_X86_64_RELATIVE 8

#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PF_DHDR (1 << 3)

static inline Elf64_Shdr *section(Elf64_Ehdr *ehdr, int n)
{
    Elf64_Shdr *shdrStart = (Elf64_Shdr *)((char *)ehdr + ehdr->e_shoff);
    return shdrStart + n;
}

static inline char *getShstrtab(Elf64_Ehdr *ehdr)
{
    return (char *)(ehdr) + section(ehdr, ehdr->e_shstrndx)->sh_offset;
}

#endif // VIPEROS_ELF_H
