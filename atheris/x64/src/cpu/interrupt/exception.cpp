#include <cpu/context.h>

#include <common/halt.h>

#include <echis/driver/console.h>

namespace atheris
{
    namespace x64
    {
        extern "C" void CommonExceptionHandler(cpu::Context*)
        {
            echis::console::Print("Exception received", 0xff0000, 0);
            atheris::Halt();
        }
    }
}