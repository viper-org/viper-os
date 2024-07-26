#include <main.h>

#include <mm/physical.h>

#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        driver::debugcon::WriteFormatted("Hello%c %s", ',', "World\n");

        auto pages = mm::physical::GetPages(6);
        mm::physical::FreePages(pages, 6);
    }
}