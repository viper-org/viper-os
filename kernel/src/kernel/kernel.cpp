#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>

#include <container/ptr.h>
#include <container/function.h>
#include <container/list.h>

extern "C" void _start()
{
    GlobalDescriptorTable::Init();
    PMM::Init();
    Paging::Init();
    mm::Init();
    Framebuffer::Init();
    Terminal::Init();

    vpr::list<int> ints;
    ints.push_back(65);
    ints.push_back(66);
    ints.push_back(67);
    ints.push_back(68);

    vpr::function<const char*()> fn = []() {
        return "hi";
    };

    [&](){
        vpr::unique_ptr<int> b = vpr::make_unique<int>(0xff0000);
        Terminal::Print(fn(), 0x00ffff, *b);
    }();

    for (auto i : ints)
    {
        Terminal::PutChar(i, 0xFFFFFF, 0);
    }

    asm volatile("1: cli; hlt; jmp 1b");
}