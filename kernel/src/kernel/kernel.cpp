#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>
#include <drivers/Apic.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>

#include <cpu/Gdt/Gdt.h>
#include <cpu/interrupt/Idt.h>
#include <cpu/interrupt/Exception.h>

#include <acpi/Acpi.h>
#include <acpi/Madt.h>

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

    Terminal::Print("test");

    asm volatile("1: cli; hlt; jmp 1b");
}