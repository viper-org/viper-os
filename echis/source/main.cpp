#include <main.h>

#include <mm/heap.h>

#include <driver/debugcon.h>

#include <atheris/mm/vm.h>

namespace echis
{
    void KernelMain()
    {
        atheris::vm::Init();
        mm::Init();

        driver::debugcon::WriteFormatted("Hello%c %s", ',', "World\n");

        int* x = new int(4);
        driver::debugcon::WriteFormatted("%p - %d", x, *x);
    }
}