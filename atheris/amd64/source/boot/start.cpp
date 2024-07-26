#include <cpu/gdt.h>
#include <cpu/idt.h>

#include <mm/pm.h>

#include <atheris/driver/debugcon.h>

#include <echis/main.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');
    atheris::amd64::cpu::gdt::Install();
    atheris::amd64::cpu::idt::Install();

    atheris::amd64::pm::Init();

    echis::KernelMain();

    for(;;) asm("pause"); // probably wont need this for long
}