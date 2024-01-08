#include <core/exception/exception.h>

#include <atheris/cpu/core.h>

#include <atheris/common/halt.h>

#include <stdio.h>

namespace echis
{
    namespace exception
    {
        constexpr const char* exceptions[] = {
            "DIV",
            "SEG",
            "ILL",
            "BRK",
            "ARC"
        };

        void raise(Exception except, int)
        {
            int b = atheris::cpu::core::id;
            printf("%#Exception received on CPU#%d: %s\n", 0xff0000, b, exceptions[except]);
            atheris::Halt();
        }
    }
}