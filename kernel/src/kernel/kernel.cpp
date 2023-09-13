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

#include <sched/Process.h>
#include <sched/Scheduler.h>

#include <fs/Tmpfs.h>
#include <fs/Vfs.h>

void test()
{
    Terminal::PutChar('A', 0xff0000, 0);
    sched::EndCurrentProcess();
}

void test2()
{
    Terminal::PutChar('B', 0x0000ff, 0);
    sched::EndCurrentProcess();
}

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
    timer::Init();

    //sched::Process process = (uint64_t)&test;
    //sched::Process process2 = (uint64_t)&test2;
    //sched::AddProcess(process);
    //sched::AddProcess(process2);
    
    //sched::Start();
    fs::tmpfs::Init();

    fs::vfs::create("tmpfs:hi");

    auto node = fs::vfs::lookup("tmpfs:hi");

    Terminal::Print(node->getPath().c_str());

    asm volatile("sti; 1: hlt; jmp 1b");
}