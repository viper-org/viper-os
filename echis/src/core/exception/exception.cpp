#include <core/exception/exception.h>

#include <driver/console.h>

#include <atheris/private/halt.h>

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
            console::Print("Exception received: ", 0xff0000, 0);
            console::Print(exceptions[except], 0xff0000, 0);
            atheris::Halt();
        }
    }
}