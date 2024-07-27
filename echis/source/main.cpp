#include <main.h>

#include <mm/physical.h>

#include <driver/debugcon.h>

#include <atheris/mm/vm.h>

namespace echis
{
    void KernelMain()
    {
        atheris::vm::Init();

        driver::debugcon::WriteFormatted("Hello%c %s", ',', "World\n");

        auto pages = mm::physical::GetPages(6);
        mm::physical::FreePages(pages, 6);
    }
}