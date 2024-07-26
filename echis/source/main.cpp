#include <main.h>

#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        driver::debugcon::WriteFormatted("Hello%c %s", ',', "World\n");

        int test = 1234;
        int b= 0;
        driver::debugcon::WriteFormatted("%i\n", test);
        driver::debugcon::WriteFormatted("%p\n", &test);
        int c = test / b;
    }
}