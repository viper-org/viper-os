#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>
#include <cpu/interrupt/Idt.h>
#include <cpu/interrupt/Exception.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    Paging::Init();
    mm::Init();
    Framebuffer::Init();
    Terminal::Init();
    idt::Init();

    exception::subscribe(0, [](InterruptFrame*){
        Terminal::Print("test\n");
        return false;
    });

    exception::subscribe(0, [](InterruptFrame*){
        Terminal::Print("test2\n");
        return true; // Failure
    });

    int a = 4;
    int b = a / 0;

    asm volatile("1: cli; hlt; jmp 1b");
}