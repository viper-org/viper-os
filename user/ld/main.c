extern unsigned long *initial_stack;

enum elf_auxval
{
    AT_ATSIZE=1,
    AT_ENTRY,
    AT_PHDR,
    AT_PHENT,
    AT_PHNUM
};

unsigned long getauxval(unsigned long type)
{
    __asm__ volatile ("syscall" :: "a"(68), "D"(initial_stack));
    return *(initial_stack - type);
}

void ld_start_main(void)
{
    void(*entry)(void) = (void(*)(void))(getauxval(AT_ENTRY));
    __asm__ volatile ("syscall" :: "a"(68), "D"(entry));
    entry();

    __asm__("1: pause; jmp 1b" ::: "memory");
}
