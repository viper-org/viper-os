#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        driver::debugcon::WriteString("Hello, World!");
    }
}