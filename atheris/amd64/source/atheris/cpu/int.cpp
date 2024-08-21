#include <atheris/cpu/int.h>

namespace atheris
{
    namespace cpu
    {
        int cliCount = 0; // TODO: Make this core-local

        void PushDisableInt()
        {
            if (!cliCount) asm("cli");
            ++cliCount;
        }

        void PopDisableInt()
        {
            --cliCount;
            if (!cliCount) asm("sti");
        }

        void DisableInt()
        {
            cliCount = 0;
            asm("sti");
        }
    }
}