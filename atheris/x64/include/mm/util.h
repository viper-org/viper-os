#ifndef VIPEROS_ATHERIS_X64_MM_UTIL_H
#define VIPEROS_ATHERIS_X64_MM_UTIL_H 1

#include <stdint.h>

namespace atheris
{
    namespace x64
    {
        uint32_t NPAGES(uint64_t n);

        uint64_t VirtToPhys(uint64_t addr);
        uint64_t PhysToVirt(uint64_t addr);

        uint64_t ValidatePageTable(uint64_t pageTable);

        uint64_t GetHHDMOffset();

        constexpr uint16_t PageSize = 0x1000;
    }
}

#endif // VIPEROS_ATHERIS_X64_MM_UTIL_H