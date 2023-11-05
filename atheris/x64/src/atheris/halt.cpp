#include <atheris/halt.h>

namespace atheris
{
    void Halt()
    {
        asm("1: cli; hlt; jmp 1b");
    }
}