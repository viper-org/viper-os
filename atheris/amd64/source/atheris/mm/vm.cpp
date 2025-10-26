// Copyright 2025 solar-mist

#include <atheris/private/mm/vm.h>

#include <limine.h>

namespace atheris
{
    namespace vm
    {
        static volatile limine_hhdm_request HHDMRequest = {
            .id = LIMINE_HHDM_REQUEST,
            .revision = 1,
            .response = nullptr
        };

        void* GetVirtualAddress(pm::physaddr_t physical)
        {
            return reinterpret_cast<void*>(physical + HHDMRequest.response->offset);
        }
    }
}