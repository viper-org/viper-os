#ifndef VIPEROS_ECHIS_MM_PHYSICAL_H
#define VIPEROS_ECHIS_MM_PHYSICAL_H 1

#include <atheris/mm/pm.h>

#include <cstdint>

namespace echis
{
    namespace mm
    {
        namespace physical
        {
            using physaddr = atheris::pm::physaddr;

            struct MemoryRegion
            {
                MemoryRegion(std::uint32_t size);

                std::uint8_t* bitmapStart;
                std::uint8_t* bitmapEnd;

                physaddr base;
                physaddr lastAllocatedPage;

                MemoryRegion* next;

                std::uint32_t totalSize;
                std::uint32_t totalFree;
            };

            void SetPageSize(unsigned int newPageSize);
            unsigned int GetPageSize();

            void AddMemoryRegion(physaddr base, std::uint32_t size);

            physaddr GetPage();
            physaddr GetPages(std::uint32_t pageCount);

            void FreePage(physaddr page);
            void FreePages(physaddr pages, std::uint32_t pageCount);

            std::uint64_t GetTotalMemory();
            std::uint64_t GetTotalFreeMemory();
        }
    }
}

#endif // VIPEROS_ECHIS_MM_PHYSICAL_H