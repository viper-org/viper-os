#include <driver/framebufferInstall.h>

#include <echis/driver/framebuffer.h>

#include <common/halt.h>

namespace x64
{
    extern "C" void kernel_main()
    {
        if (InstallFramebuffer() == FramebufferInstallError)
        {
            atheris::Halt();
        }
        echis::framebuffer::PutPixel(10, 10, 0xff0000);
        atheris::Halt();
    }
}