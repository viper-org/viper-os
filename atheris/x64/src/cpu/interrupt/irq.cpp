#include <cpu/context.h>
#include <cpu/core.h>
#include <common/halt.h>

#include <stdio.h>

namespace atheris
{
    namespace x64
    {
        extern "C" void CommonIRQHandler(cpu::Context* context)
        {
            if (context->BaseFrame.vector == 255)
            {
                int id = atheris::cpu::core::id;
                printf("%#Halt IPI received on CPU#%i\n", 0xff0000, id);
                atheris::Halt();
            }
        }
    }
}