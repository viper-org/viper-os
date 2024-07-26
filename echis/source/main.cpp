#include <main.h>

#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        driver::debugcon::Write("Hello World");
    }
}