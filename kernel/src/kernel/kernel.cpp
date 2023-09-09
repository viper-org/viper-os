#include <cpu/Gdt/Gdt.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    asm volatile("1: cli; hlt; jmp 1b");
}