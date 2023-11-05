#include <driver/framebufferInstall.h>

#include <echis/driver/framebuffer.h>

#include <common/halt.h>

namespace x64
{
    extern "C" void kernel_main()
    {
        unsigned char bitmap[] = {
            0xff,
            0xff,
            0x00,
            0xff,
            0xff,
            0x00,
            0xff,
            0xff
        };

        if (InstallFramebuffer() == FramebufferInstallError)
        {
            atheris::Halt();
        }
        echis::framebuffer::PutPixel(10, 10, 0xff0000);
        echis::framebuffer::DrawBitmap(bitmap, 100, 100, 8, 8, 0x00ffff, 0xff0000);
        atheris::Halt();
    }
}