#include <elf.h>
#include <syscall.h>

extern unsigned long *initial_stack;
extern void resolve_sym_i(void);
uint64_t resolve_sym(uint64_t got, uint32_t idx);

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

char *strtab = NULL;
Elf64_Sym *symtab = NULL;
struct Elf64_Rela *jmprel = NULL;
uint64_t execGot = 0;

struct shared_library *libs = NULL;
int lib_ptr = 0;

void init_library(const char *path)
{
    struct stat b;
    stat(path, &b);
    void *mem = mmap(b.st_size);
    int fd = open(path, 3);
    read(fd, mem, b.st_size);

    struct elf_exec e;
    load_library(mem, &e);
    libs[lib_ptr].e = e;

    struct Elf64_Ehdr *ehdr = mem;
    struct Elf64_Phdr *phdrs = (struct Elf64_Phdr *)((char *)mem + ehdr->e_phoff);
    struct Elf64_Phdr *dynamic = NULL;
    for (uint64_t i = 0; i < ehdr->e_phnum; ++i)
    {
        struct Elf64_Phdr *phdr = phdrs + i;

        if (phdr->p_type == PT_DYNAMIC) dynamic = phdr;
    }

    struct Elf64_Dyn *tag = (struct Elf64_Dyn *)(e.at_base1 + dynamic->p_vaddr);
    int bind_now = 0;
    while (tag->d_tag != DT_NULL)
    {
        if (tag->d_tag == DT_PLTGOT) libs[lib_ptr].got = tag->d_un.d_ptr;
        else if (tag->d_tag == DT_PLTRELSZ) libs[lib_ptr].pltrelsz = tag->d_un.d_val;
        else if (tag->d_tag == DT_STRTAB) libs[lib_ptr].strtab = (char *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_SYMTAB) libs[lib_ptr].symtab = (struct Elf64_Sym *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_JMPREL) libs[lib_ptr].jmprel = (struct Elf64_Rela *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_GNU_HASH) libs[lib_ptr].hashtab = (uint32_t *)(tag->d_un.d_ptr + e.at_base1);
        else if (tag->d_tag == DT_FLAGS)
        {
            if (tag->d_un.d_val & DT_BIND_NOW) bind_now = 1;
        }

        ++tag;
    }

    libs[lib_ptr].got += e.at_base1;
    *((uint64_t *)libs[lib_ptr].got + 1) = libs[lib_ptr].got;
    *((uint64_t *)libs[lib_ptr].got + 2) = (uint64_t)resolve_sym_i;

    for (uint64_t i = 0; i < libs[lib_ptr].pltrelsz / sizeof(struct Elf64_Rela); ++i)
    {
        *((uint64_t *)libs[lib_ptr].got + 3 + i) += e.at_base1;
        if (bind_now)
        {
            resolve_sym(libs[lib_ptr].got, i);
        }
    }

    ++lib_ptr;
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

uint64_t resolve_sym_impl(uint64_t got, char *strtab, struct Elf64_Sym *symtab, struct Elf64_Rela *jmprel, uint32_t idx)
{
    struct Elf64_Rela *rela = jmprel + idx;
    char *sym_name = strtab + symtab[ELF64_R_SYM(rela->r_info)].st_name;
    print("Resolving symbol: ");
    print(sym_name);
    print("\n");
    for (int i = 0; i < lib_ptr; ++i)
    {
        struct shared_library *lib = &libs[i];
        struct Elf64_Sym *sym = lookup(lib->strtab, lib->symtab, lib->hashtab, sym_name);
        if (sym)
        {
            uint64_t real_addr = sym->st_value + lib->e.at_base1;
            *((uint64_t *)got + idx + 3) = real_addr;
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
    if (got == execGot)
        return resolve_sym_impl(got, strtab, symtab, jmprel, idx);
    
    for (int i = 0; i < lib_ptr; ++i)
    {
        if (libs[i].got == got)
        {
            return resolve_sym_impl(libs[i].got, libs[i].strtab, libs[i].symtab, libs[i].jmprel, idx);
        }
    }
    print("Failed to find GOT: ");
    printp((void*)got);
    return 0xCA11CA11CA11CA11;
}

void ld_start_main(void)
{
    libs = mmap(0x100 * sizeof(struct shared_library));

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
    int bind_now = 0;
    while (tag->d_tag != DT_NULL)
    {
        if (tag->d_tag == DT_PLTGOT) execGot = tag->d_un.d_ptr;
        else if (tag->d_tag == DT_PLTRELSZ) pltrelsz = tag->d_un.d_val;
        else if (tag->d_tag == DT_SYMTAB) symtab = (Elf64_Sym *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_JMPREL) jmprel = (struct Elf64_Rela *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_STRTAB) strtab = (char *)(tag->d_un.d_ptr + base1);
        else if (tag->d_tag == DT_FLAGS)
        {
            if (tag->d_un.d_val & DT_BIND_NOW) bind_now = 1;
        }
        ++tag;
    }

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

    execGot += base1;
    *((uint64_t *)execGot + 1) = execGot;
    *((uint64_t *)execGot + 2) = (uint64_t)resolve_sym_i;

    for (uint64_t i = 0; i < pltrelsz / sizeof(struct Elf64_Rela); ++i)
    {
        *((uint64_t *)execGot + 3 + i) += base1;
        if (bind_now)
        {
            resolve_sym(execGot, i);
        }
    }

    
    void(*entry)(void) = (void(*)(void))(getauxval(AT_ENTRY));
    entry();

    __asm__("1: pause; jmp 1b" ::: "memory");
}
