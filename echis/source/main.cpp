#include <main.h>

#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        driver::debugcon::WriteFormatted("Hello%c %s", ',', "World");
        
        int test = 1234;
        driver::debugcon::WriteFormatted("\n%i", test);
        driver::debugcon::WriteFormatted("\n%p", &test);
    }
}