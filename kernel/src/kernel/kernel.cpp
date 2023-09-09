#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>

#include <cpu/Gdt/Gdt.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    Paging::Init();

    Framebuffer::Init();
    Terminal::Init();
    Terminal::Print("Hello World", 0x00ffff, 0);

    asm volatile("1: cli; hlt; jmp 1b");
}