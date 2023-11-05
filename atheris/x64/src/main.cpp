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
        echis::console::PutChar('H', 0xffffff, 0);
        echis::console::PutChar('e', 0xffffff, 0);
        echis::console::PutChar('l', 0xffffff, 0);
        echis::console::PutChar('l', 0xffffff, 0);
        echis::console::PutChar('o', 0xffffff, 0);
        atheris::Halt();
    }
}