#include <atheris/private/cpu/halt.h>

namespace atheris
{
    namespace cpu
    {
        void Halt()
        {
            asm("1: hlt; jmp 1b");
        }
    }
}