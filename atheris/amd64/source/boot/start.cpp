#include <cpu/gdt.h>

#include <atheris/driver/debugcon.h>

#include <echis/main.h>

extern "C" void _start()
{
    atheris::driver::debugcon::WriteChar('\n');
    atheris::amd64::cpu::gdt::Install();
    echis::KernelMain();

    for(;;) asm("pause"); // probably wont need this for long
}