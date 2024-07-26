#include <atheris/mm/vm.h>

#include <limine.h>

static volatile limine_hhdm_request hhdmRequest = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 1,
    .response = nullptr
};

namespace atheris
{
    namespace vm
    {
        void* GetVirtualAddress(pm::physaddr physAddress)
        {
            // maybe check that response is valid here
            return reinterpret_cast<void*>(hhdmRequest.response->offset + physAddress);
        }
    }
}