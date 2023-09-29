#include <drivers/Framebuffer.h>
#include <drivers/Terminal.h>
#include <drivers/Apic.h>
#include <drivers/Hpet.h>
#include <drivers/ApicTimer.h>

#include <mm/Pmm.h>
#include <mm/Paging.h>
#include <mm/Heap.h>
#include <mm/Vmm.h>

#include <cpu/Gdt/Gdt.h>
#include <cpu/Gdt/Tss.h>
#include <cpu/interrupt/Idt.h>
#include <cpu/interrupt/Exception.h>
#include <cpu/interrupt/Irq.h>

#include <acpi/Acpi.h>
#include <acpi/Madt.h>

#include <sched/Process.h>
#include <sched/Scheduler.h>

#include <fs/Tmpfs.h>
#include <fs/DevFs.h>
#include <fs/Vfs.h>

#include <string.h>

void test()
{
    Terminal::Print("hello from process 1\n");
    while(1) asm("hlt");
}

void test2()
{
    Terminal::Print("hello from process 2\n");
    while(1) asm("hlt");
}

extern "C" void _start()
{
    PMM::Init();
    paging::Init();
    mm::Init();
    vm::Init();

    gdt::Init();
    tss::Init();
    idt::Init();

    Framebuffer::Init();
    Terminal::Init();

    ACPI::Init();
    MADT::Init();

    APIC::Init();

    HPET::Init();
    timer::Init();

    fs::tmpfs::Init();
    fs::devfs::Init();

    sched::Process* testProc  = new sched::Process((uint64_t)&test);
    sched::Process* testProc2 = new sched::Process((uint64_t)&test2);
    sched::AddProcess(*testProc);
    sched::AddProcess(*testProc2);
    sched::Start();

    asm volatile("cli; 1: hlt; jmp 1b");
}