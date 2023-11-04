#ifndef VIPEROS_ECHIS_MM_PMM_H
#define VIPEROS_ECHIS_MM_PMM_H 1

#include <stdint.h>

namespace echis
{
    namespace pmm
    {
        using physaddr = uint64_t;

        void AddRegion(physaddr base, uint32_t size);

        physaddr GetPage();
        physaddr GetPages(uint32_t pageCount);

        void FreePage(physaddr page);
        void FreePages(physaddr page, uint32_t pageCount);
    }
}

#endif // VIPEROS_ECHIS_MM_PMM_H