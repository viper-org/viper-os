#include "cpu/cpu.h"
#include "cpu/gdt.h"
#include "cpu/tss.h"
#include "cpu/idt.h"
#include "cpu/asm.h"

void cpu_init(void)
{
    gdt_init();
    idt_init();
    tss_init();

    uint64_t efer = rdmsr(IA32_EFER);
    wrmsr(IA32_EFER, efer | 1); // SCE
}
