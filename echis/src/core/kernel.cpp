#include "driver/console.h"
#include <atheris/driver/framebufferInit.h>
#include <atheris/driver/consoleInit.h>

#include <atheris/common/halt.h>

namespace echis
{
    void kernel_main()
    {
        atheris::framebuffer::Init();
        atheris::console::Init();

        echis::console::Print("Hello, World!\n", 0x00ffff, 0);

        atheris::Halt();
    }
}