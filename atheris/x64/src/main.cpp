#include <cpu/gdt/gdt.h>
#include <cpu/interrupt/idt.h>

#include <driver/framebufferInstall.h>
#include <driver/consoleInstall.h>

#include <echis/driver/framebuffer.h>
#include <echis/driver/console.h>

#include <common/halt.h>

namespace x64
{
    extern "C" void kernel_main()
    {
        cpu::gdt::Install();
        cpu::interrupt::Install();

        if (InstallFramebuffer() == FramebufferInstallError)
        {
            atheris::Halt();
        }
        if (InstallConsole() == ConsoleInstallError)
        {
            atheris::Halt();
        }
        echis::console::Print("Hello world!\n", 0x00ffff, 0);

        atheris::Halt();
    }
}