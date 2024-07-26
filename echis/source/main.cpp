#include <main.h>

#include <atheris/driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        atheris::driver::debugcon::Write("Hello World");
    }
}