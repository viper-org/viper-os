#include <driver/console.h>

#include <atheris/driver/framebuffer.h>
#include <atheris/driver/console.h>

#include <atheris/mm/pm.h>
#include <atheris/mm/vm.h>

#include <atheris/common/halt.h>

namespace echis
{
    void kernel_main()
    {
        atheris::framebuffer::Init();
        atheris::console::Init();

        atheris::pm::Init();
        atheris::vm::Init();

        echis::console::Print("Hello, World!\n", 0x00ffff, 0);

        atheris::Halt();
    }
}