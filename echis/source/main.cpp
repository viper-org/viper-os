#include "atheris/private/mm/vm.h"
#include "mm/physical.h"
#include <driver/debugcon.h>

namespace echis
{
    void KernelMain()
    {
        auto page = mm::physical::GetPage();
        void* virt = atheris::vm::GetVirtualAddress(page);
        *(const char**)virt = "Hello";

        driver::debugcon::WriteString(*(const char**)virt);

        mm::physical::FreePage(page);
    }
}