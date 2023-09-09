#include "mm/Pmm.h"
#include <cpu/Gdt/Gdt.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    asm volatile("1: cli; hlt; jmp 1b");
}