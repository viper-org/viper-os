#include <elf.h>
#include <syscall.h>

extern unsigned long *initial_stack;
extern void resolve_sym_i(void);
uint64_t resolve_sym(uint64_t got, uint32_t idx);
uint64_t resolve_sym_impl(struct shared_library *dest, struct Elf64_Rela *rela, uint32_t idx);

unsigned long getauxval(unsigned long type)
{
    return *(initial_stack - type);
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2)
    {
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}

void *memcpy(void *d, const void *s, size_t size)
{
    char *d1 = d;
    const char *s1 = s;
    while(size--)
    {
        *d1 = *s1;
        ++d1;
        ++s1;
    }
    return d;
}

char *strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while (*s2)
    {
        *s1 = *s2;
        ++s1;
        ++s2;
    }
    *s1 = 0;
    return s;
}

struct Elf64_Rela *jmprel = NULL;

struct shared_library *libs = NULL;
int lib_ptr = 0;
struct copy_relocation *copies = NULL;
int copy_ptr = 0;
struct shared_library exec;

void do_library_relocations(struct shared_library *lib)
{
    for (uint64_t i = 0; i < lib->pltrelsz / sizeof(struct Elf64_Rela); ++i)
    {
        *((uint64_t *)lib->got + 3 + i) += lib->e.at_base1;
        if (lib->bind_now)
        {
            resolve_sym(lib->got, i);
        }
    }

    struct Elf64_Rela *rela = (struct Elf64_Rela *)lib->rela_addr;
    while ((char *)rela < (char *)lib->rela_addr + lib->relasz)
    {
        switch (ELF64_R_TYPE(rela->r_info))
        {
            case R_X86_64_COPY:
            case R_X86_64_GLOB_DAT:
            {
                resolve_sym_impl(lib, rela, 0);
                break;
            }
            default:
                break;
        }
        ++rela;
    }
}

void init_library(const char *path)
{
    struct stat b;
    stat(path, &b);
    void *mem = mmap(b.st_size);
    int fd = open(path, 3);
    read(fd, mem, b.st_size);

    int lib_idx = lib_ptr++;

    struct elf_exec e;
    load_library(mem, &e);
    libs[lib_idx].e = e;

    struct Elf64_Ehdr *ehdr = mem;
    struct Elf64_Phdr *phdrs = (struct Elf64_Phdr *)((char *)mem + ehdr->e_phoff);
    struct Elf64_Phdr *dynamic = NULL;
    for (uint64_t i = 0; i < ehdr->e_phnum; ++i)
    {
        struct Elf64_Phdr *phdr = phdrs + i;

        if (phdr->p_type == PT_DYNAMIC) dynamic = phdr;
    }

    struct Elf64_Dyn *tag = (struct Elf64_Dyn *)(e.at_base1 + dynamic->p_vaddr);
    while (tag->d_tag != DT_NULL)
    {
        if (tag->d_tag == DT_PLTGOT) libs[lib_idx].got = tag->d_un.d_ptr;
        else if (tag->d_tag == DT_PLTRELSZ) libs[lib_idx].pltrelsz = tag->d_un.d_val;
        else if (tag->d_tag == DT_STRTAB) libs[lib_idx].strtab = (char *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_SYMTAB) libs[lib_idx].symtab = (struct Elf64_Sym *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_JMPREL) libs[lib_idx].jmprel = (struct Elf64_Rela *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_GNU_HASH) libs[lib_idx].hashtab = (uint32_t *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_RELA) libs[lib_idx].rela_addr = tag->d_un.d_ptr + e.at_base1;
        else if (tag->d_tag == DT_RELASZ) libs[lib_idx].relasz = tag->d_un.d_val;
        else if (tag->d_tag == DT_FLAGS)
        {
            if (tag->d_un.d_val & DT_BIND_NOW) libs[lib_idx].bind_now = 1;
        }

        ++tag;
    }

    libs[lib_idx].got += e.at_base1;
    *((uint64_t *)libs[lib_idx].got + 1) = libs[lib_idx].got;
    *((uint64_t *)libs[lib_idx].got + 2) = (uint64_t)resolve_sym_i;

    close(fd);
    // munmap(mem)
}

uint32_t gnu_hash(const char *s)
{
    uint32_t h = 5381;

    for (unsigned char c = *s; c != '\0'; c = *++s)
            h = h * 33 + c;

    return h;
}

struct Elf64_Sym *lookup(char *strtab, struct Elf64_Sym *symtab, uint32_t *hashtab, char *name)
{
    uint32_t namehash = gnu_hash(name);

    uint32_t nbuckets = hashtab[0];
    uint32_t symoffset = hashtab[1];
    uint32_t bloom_size = hashtab[2];
    uint32_t bloom_shift = hashtab[3];
    uint64_t *bloom = (void *)&hashtab[4];
    uint32_t *buckets = (void *)&bloom[bloom_size];
    uint32_t *chain = &buckets[nbuckets];

    uint64_t word = bloom[(namehash / 64) % bloom_size];
    uint64_t mask = 0 | (uint64_t)1 << (namehash % 64) | (uint64_t)1 << ((namehash >> bloom_shift) % 64);

    if ((word & mask) != mask) return NULL;

    uint32_t symix = buckets[namehash % nbuckets];
    if (symix < symoffset) return NULL;

    for (;;)
    {
        char *symname = strtab + symtab[symix].st_name;
        uint32_t hash = chain[symix - symoffset];

        if ((namehash | 1) == (hash | 1) && !strcmp(name, symname))
            return &symtab[symix];

        if (hash & 1) break;
        
        ++symix;
    }
    return NULL;
}

uint64_t resolve_sym_impl(struct shared_library *dest, struct Elf64_Rela *rela, uint32_t idx)
{
    char *sym_name = dest->strtab + dest->symtab[ELF64_R_SYM(rela->r_info)].st_name;
    print("Resolving symbol: ");
    print(sym_name);
    print("\n");
    for (int i = 0; i < copy_ptr; ++i)
    {
        if (!strcmp(copies[i].name, sym_name))
        {
            if (ELF64_R_TYPE(rela->r_info) == R_X86_64_GLOB_DAT)
            {
                uint64_t real_addr = copies[i].address;
                *((uint64_t *)(rela->r_offset + dest->e.at_base1)) = real_addr;
                return 0xDEADBEEF;
            }
            uint64_t real_addr = copies[i].address;
            *((uint64_t *)dest->got + idx + 3) = real_addr;
            return real_addr;
        }
    }
    for (int i = 0; i < lib_ptr; ++i)
    {
        struct shared_library *lib = &libs[i];
        struct Elf64_Sym *sym = lookup(lib->strtab, lib->symtab, lib->hashtab, sym_name);
        if (sym)
        {
            if (ELF64_R_TYPE(rela->r_info) == R_X86_64_COPY)
            {
                memcpy((char *)rela->r_offset + dest->e.at_base1, (char *)sym->st_value + lib->e.at_base1, sym->st_size);
                copies[copy_ptr].name = sym_name;
                copies[copy_ptr++].address = rela->r_offset + dest->e.at_base1;
                return 0xDEADBEEF;
            }
            else if (ELF64_R_TYPE(rela->r_info) == R_X86_64_GLOB_DAT)
            {
                uint64_t real_addr = sym->st_value + lib->e.at_base1;
                *((uint64_t *)(rela->r_offset + dest->e.at_base1)) = real_addr;
                return 0xDEADBEEF;
            }
            uint64_t real_addr = sym->st_value + lib->e.at_base1;
            *((uint64_t *)dest->got + idx + 3) = real_addr;
            return real_addr;
        }
    }
    print("Failed to resolve symbol: ");
    print(sym_name);
    print("\n");
    return 0xCA11CA11CA11CA11;
}

uint64_t resolve_sym(uint64_t got, uint32_t idx)
{
    if (got == exec.got)
        return resolve_sym_impl(&exec, jmprel+idx, idx);
    
    for (int i = 0; i < lib_ptr; ++i)
    {
        if (libs[i].got == got)
        {
            return resolve_sym_impl(&libs[i], libs[i].jmprel+idx, idx);
        }
    }
    print("Failed to find GOT: ");
    printp((void*)got);
    return 0xCA11CA11CA11CA11;
}

void ld_start_main(void)
{
    libs = mmap(0x100 * sizeof(struct shared_library));
    copies = mmap(0x100 * sizeof(struct copy_relocation));

    struct Elf64_Phdr *phdrs = (struct Elf64_Phdr *)getauxval(AT_PHDR);
    struct Elf64_Phdr *dynamic = NULL;
    uint64_t phnum = getauxval(AT_PHNUM);
    for (uint64_t i = 0; i < phnum; ++i)
    {
        struct Elf64_Phdr *phdr = phdrs + i;

        if (phdr->p_type == PT_DYNAMIC) dynamic = phdr;
    }
    
    uint64_t base1 = getauxval(AT_BASE1);
    struct Elf64_Dyn *tag = (struct Elf64_Dyn *)(base1 + dynamic->p_vaddr);
    uint64_t pltrelsz = 0;
    uint64_t rela_addr;
    uint64_t relasz;
    uint64_t execGot;
    char *strtab;
    Elf64_Sym *symtab;
    int bind_now = 0;
    while (tag->d_tag != DT_NULL)
    {
        if (tag->d_tag == DT_PLTGOT) execGot = tag->d_un.d_ptr;
        else if (tag->d_tag == DT_PLTRELSZ) pltrelsz = tag->d_un.d_val;
        else if (tag->d_tag == DT_SYMTAB) symtab = (Elf64_Sym *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_JMPREL) jmprel = (struct Elf64_Rela *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_STRTAB) strtab = (char *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_RELA) rela_addr = tag->d_un.d_ptr + base1;
        else if (tag->d_tag == DT_RELASZ) relasz = tag->d_un.d_val;
        else if (tag->d_tag == DT_FLAGS)
        {
            if (tag->d_un.d_val & DT_BIND_NOW) bind_now = 1;
        }
        ++tag;
    }

    execGot += base1;
    *((uint64_t *)execGot + 1) = execGot;
    *((uint64_t *)execGot + 2) = (uint64_t)resolve_sym_i;

    exec.e.at_base1 = base1;
    exec.got = execGot;
    exec.jmprel = jmprel;
    exec.pltrelsz = pltrelsz;
    exec.strtab = strtab;
    exec.symtab = symtab;

    tag = (struct Elf64_Dyn *)(base1 + dynamic->p_vaddr);
    while (tag->d_tag != DT_NULL)
    {
        if (tag->d_tag == DT_NEEDED)
        {
            char fullpath[100] = "/lib/";
            char *path = strtab + tag->d_un.d_val;
            strcpy(fullpath + 5, path);
            init_library(fullpath);
        }
        ++tag;
    }

    for (uint64_t i = 0; i < pltrelsz / sizeof(struct Elf64_Rela); ++i)
    {
        *((uint64_t *)execGot + 3 + i) += base1;
        if (bind_now)
        {
            resolve_sym(execGot, i);
        }
    }

    struct Elf64_Rela *rela = (struct Elf64_Rela *)rela_addr;
    while ((char *)rela < (char *)rela_addr + relasz)
    {
        switch (ELF64_R_TYPE(rela->r_info))
        {
            case R_X86_64_COPY:
            case R_X86_64_GLOB_DAT:
            {
                resolve_sym_impl(&exec, rela, 0);
                break;
            }
            default:
                break;
        }
        ++rela;
    }

    for (int i = 0; i < lib_ptr; ++i)
    {
        do_library_relocations(libs + i);
    }

    void(*entry)(void) = (void(*)(void))(getauxval(AT_ENTRY));
    entry();

    __asm__("1: pause; jmp 1b" ::: "memory");
}
