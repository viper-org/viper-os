#include "cpu/cpu.h"
#include "cpu/gdt.h"
#include "cpu/tss.h"
#include "cpu/idt.h"
#include "cpu/asm.h"
#include "mm/kheap.h"

void syscall_handler(void);

static struct core_struct *core;

void cpu_earlyinit(void)
{
    gdt_init();
    idt_init();
    tss_init();
}

void cpu_init(void)
{
    uint64_t efer = rdmsr(IA32_EFER);
    wrmsr(IA32_EFER, efer | 1); // SCE
    wrmsr(IA32_STAR, ((uint64_t)0x8 << 32) | ((uint64_t)0x13 << 48));
    wrmsr(IA32_LSTAR, (uint64_t)syscall_handler);
    wrmsr(IA32_SFMASK, 0x200);

    core = kheap_alloc(sizeof (struct core_struct));
    core->self = core;
    wrmsr(GS_BASE, (uint64_t)core);
}

struct core_struct *get_core(void)
{
    return core;
}
