#include <atheris/private/halt.h>

namespace atheris
{
    void Halt()
    {
        asm("1: hlt; jmp 1b");
    }
}