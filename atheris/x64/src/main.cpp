#include <cpu/gdt/gdt.h>
#include <cpu/interrupt/idt.h>

#include <echis/core/kernel.h>

#include <echis/driver/framebuffer.h>
#include <echis/driver/console.h>

#include <common/halt.h>

namespace x64
{
    extern "C" void kernel_init()
    {
        cpu::gdt::Install();
        cpu::interrupt::Install();

        echis::kernel_main();
        atheris::Halt();
    }
}