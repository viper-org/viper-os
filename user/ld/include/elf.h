#ifndef VIPEROS_LD_ELF_H
#define VIPEROS_LD_ELF_H 1

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
#define DT_NEEDED 1
#define DT_PLTRELSZ 2
#define DT_PLTGOT 3
#define DT_STRTAB 5
#define DT_SYMTAB 6
#define DT_RELA 7
#define DT_RELASZ 8
#define DT_RELAENT 9
#define DT_JMPREL 23
#define DT_FLAGS 30
#define DT_GNU_HASH 0x6FFFFEF5

#define DT_BIND_NOW 8

struct Elf64_Rela
{
    Elf64_Addr r_offset;
    Elf64_Xword r_info;
    Elf64_Sxword r_addend;
};

#define ELF64_R_TYPE(info) ((Elf64_Word)(info))
#define ELF64_R_SYM(info) ((info) >> 32)

#define R_X86_64_COPY 5
#define R_X86_64_GLOB_DAT 6
#define R_X86_64_RELATIVE 8

#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_PHDR 6
#define PF_DHDR (1 << 3)


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

struct shared_library
{
    struct elf_exec e;

    char *strtab;
    struct Elf64_Sym *symtab;
    struct Elf64_Rela *jmprel;
    uint32_t *hashtab;
    uint64_t got;
    uint64_t pltrelsz;

    int bind_now;
    uint64_t rela_addr;
    uint64_t relasz;
};

struct copy_relocation
{
    char *name;
    uint64_t address;
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

#endif // VIPEROS_LD_ELF_H
