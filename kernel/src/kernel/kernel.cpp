#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>

#include <container/ptr.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    Paging::Init();
    mm::Init();
    Framebuffer::Init();
    Terminal::Init();

    [](){
        vpr::unique_ptr<int> b = vpr::make_unique<int>(0xff0000);
        Terminal::Print("Hello World", 0x00ffff, *b);
    }();

    asm volatile("1: cli; hlt; jmp 1b");
}