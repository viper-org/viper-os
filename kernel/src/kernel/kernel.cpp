#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>

#include <memory.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    Paging::Init();
    mm::Init();

    int* a = new int(0xff0000);

    Framebuffer::Init();
    Terminal::Init();
    Terminal::Print("Hello World", 0x00ffff, *a);
    
    delete a;

    asm volatile("1: cli; hlt; jmp 1b");
}