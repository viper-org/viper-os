#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>
#include <drivers/Apic.h>
#include <drivers/Hpet.h>
#include <drivers/ApicTimer.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>
#include <cpu/interrupt/Idt.h>
#include <cpu/interrupt/Exception.h>
#include <cpu/interrupt/Irq.h>

#include <acpi/Acpi.h>
#include <acpi/Madt.h>

#include <container/ring.h>

extern "C" void _start()
{
    PMM::Init();
    Paging::Init();
    mm::Init();

    GlobalDescriptorTable::Init();
    idt::Init();

    Framebuffer::Init();
    Terminal::Init();

    ACPI::Init();
    MADT::Init();

    APIC::Init();

    HPET::Init();

    timer::subscribe([](){
        Terminal::Print("timer\n");
    });

    timer::Init();

    asm volatile("sti; 1: hlt; jmp 1b");
}