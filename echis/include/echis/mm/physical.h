#ifndef VIPEROS_ECHIS_MM_PHYSICAL_H
#define VIPEROS_ECHIS_MM_PHYSICAL_H 1

#include <atheris/mm/pm.h>

namespace echis
{
    namespace mm
    {
        namespace physical
        {
            using physaddr_t = atheris::pm::physaddr_t;
            constexpr auto PageSize = atheris::pm::PageSize;

            struct MemoryRegion
            {
                MemoryRegion(uint32_t length);

                uint8_t* bmpStart;
                uint8_t* bmpEnd;

                physaddr_t base;
                physaddr_t lastAllocation; // cache last allocated page

                MemoryRegion* nextRegion;

                uint32_t totalSize;
                uint32_t totalFree;
            };

            void AddMemoryRegion(physaddr_t base, uint32_t length);

            physaddr_t GetPage();
            // This should be used rarely since virtual pages can be allocated backed by non-contiguous pages
            physaddr_t GetPages(uint32_t count);

            void FreePage(physaddr_t page);
            void FreePages(physaddr_t pages, uint32_t count);

            uint64_t GetTotalMemory();
            uint64_t GetTotalFreeMemory();
        }
    }
}

#endif // VIPEROS_ECHIS_MM_PHYSICAL_H