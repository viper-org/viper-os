#include <atheris/cpu/halt.h>

#include <utility>

namespace atheris
{
    namespace cpu
    {
        void Halt()
        {
            asm volatile("1: cli; hlt; jmp 1b");
            std::unreachable();
        }
    }
}