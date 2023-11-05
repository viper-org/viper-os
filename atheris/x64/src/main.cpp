#include <driver/framebufferInstall.h>
#include <driver/consoleInstall.h>

#include <echis/driver/framebuffer.h>
#include <echis/driver/console.h>

#include <common/halt.h>

namespace x64
{
    extern "C" void kernel_main()
    {
        if (InstallFramebuffer() == FramebufferInstallError)
        {
            atheris::Halt();
        }
        if (InstallConsole() == ConsoleInstallError)
        {
            atheris::Halt();
        }
        echis::console::Print("Hello world!", 0x00ffff, 0);
        atheris::Halt();
    }
}