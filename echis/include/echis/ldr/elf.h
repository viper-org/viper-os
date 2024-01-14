#ifndef VIPEROS_ECHIS_LDR_ELF_H
#define VIPEROS_ECHIS_LDR_ELF_H 1

#include <atheris/mm/vm.h>

#include <stdint.h>

namespace echis
{
    namespace elf
    {
        struct Executable
        {
            uint64_t entryPoint;
        };

        Executable Load(void* addr, atheris::vm::AddressSpace* addressSpace);
    }
}

#endif // VIPEROS_ECHIS_LDR_ELF_H