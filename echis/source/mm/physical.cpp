#include "util/math.h"
#include <mm/physical.h>

#include <atheris/mm/vm.h>

#include <new>
#include <string.h>

namespace echis
{
    namespace mm
    {
        namespace physical
        {
            static MemoryRegion* freeList = nullptr;

            static inline void PushMemoryRegion(MemoryRegion* node)
            {
                if (!freeList)
                {
                    freeList = node;
                }
                else
                {
                    node->nextRegion = freeList;
                    freeList = node;
                }
            }
            MemoryRegion::MemoryRegion(uint32_t length)
                : lastAllocation(0)
                , nextRegion(nullptr)
                , totalSize(length)
            {
            }

            void AddMemoryRegion(physaddr_t base, uint32_t length)
            {
                void* virtualBase = atheris::vm::GetVirtualAddress(base);

                MemoryRegion* region = new(virtualBase) MemoryRegion(length);

                uint32_t bmpSize = (length - sizeof(MemoryRegion)) / (PageSize * 8); // TODO: Replace 8 with CHAR_BIT
                uint8_t* bmpStart = static_cast<uint8_t*>(virtualBase) + sizeof(MemoryRegion);
                region->bmpStart = new(bmpStart) uint8_t[bmpSize];
                region->bmpEnd = region->bmpStart + bmpSize;

                region->base = util::AlignUp(base + sizeof(MemoryRegion) + bmpSize, PageSize);
                region->totalSize -= (region->base - base);
                region->totalFree = region->totalSize;

                memset(region->bmpStart, 0xFF, bmpSize);

                PushMemoryRegion(region);
            }

            static inline bool BitmapTest(MemoryRegion* region, physaddr_t address, uint32_t pageCount)
            {
                bool result = true;
                uint64_t index = address - region->base;

                uint64_t endIndex = (index + (pageCount * PageSize - 1)) / (PageSize * 8);
                if (&region->bmpStart[endIndex] >= region->bmpEnd)
                {
                    return false;
                }

                for (uint64_t i = index; i < index + (pageCount * PageSize); i += PageSize)
                {
                    result = region->bmpStart[i / (PageSize * 8)] & (1 << ((i / PageSize) % 8));
                    if (!result) break;
                }

                return result;
            }

            static inline void BitmapSet(MemoryRegion* region, physaddr_t address, uint32_t pageCount)
            {
                uint64_t index = address - region->base;

                for (uint64_t i = index; i < index + (pageCount * PageSize); i += PageSize)
                {
                    region->bmpStart[i / (PageSize * 8)] &= ~(1 << ((i / PageSize) % 8));
                }
            }

            
            bool TryAllocateAt(MemoryRegion* region, physaddr_t address, uint32_t pageCount)
            {
                if (!BitmapTest(region, address, pageCount))
                {
                    return false;
                }

                BitmapSet(region, address, pageCount);
                region->totalFree -= pageCount * PageSize;
                region->lastAllocation = address + pageCount * PageSize;
                return true;
            }

            physaddr_t CheckRegionFrom(MemoryRegion* region, physaddr_t start, uint32_t pageCount)
            {
                const physaddr_t regionEnd = region->base + region->totalSize;

                for (physaddr_t address = start; address < regionEnd; address += PageSize)
                {
                    if (TryAllocateAt(region, address, pageCount))
                    {
                        return address;
                    }
                }

                return 0;
            }

            physaddr_t CheckRegion(MemoryRegion* region, uint32_t pageCount)
            {
                if (region->lastAllocation != 0)
                {
                    if (physaddr_t address = CheckRegionFrom(region, region->lastAllocation, pageCount))
                    {
                        return address;
                    }
                }
                if (physaddr_t address = CheckRegionFrom(region, region->base, pageCount))
                {
                    return address;
                }

                return 0;
            }

            physaddr_t GetPage()
            {
                return GetPages(1);
            }
            physaddr_t GetPages(uint32_t pageCount)
            {
                if (pageCount == 0)
                {
                    return 0;
                }

                MemoryRegion* currentRegion = freeList;
                while (currentRegion != nullptr)
                {
                    if (physaddr_t address = CheckRegion(currentRegion, pageCount))
                    {
                        return address;
                    }

                    currentRegion = currentRegion->nextRegion;
                }

                return 0; // No pages found
            }


            void FreePageInRegion(MemoryRegion* region, physaddr_t page)
            {
                if (!BitmapTest(region, page, 1))
                {
                    region->totalFree += PageSize;
                }

                uint64_t index = page - region->base;
                region->bmpStart[index / (PageSize / 8)] |= ((index / PageSize) % 8);
            }

            bool TryFreePagesInRegion(MemoryRegion* region, physaddr_t pages, uint32_t pageCount)
            {
                physaddr_t pagesEnd = pages + (pageCount * PageSize);
                physaddr_t regionEnd = region->base + region->totalSize;

                if (pages >= region->base && pagesEnd <= regionEnd)
                {
                    for (physaddr_t page = pages; page < pagesEnd; page += PageSize)
                    {
                        FreePageInRegion(region, page);
                    }
                    return true;
                }

                return false;
            }

            void FreePage(physaddr_t page)
            {
                FreePages(page, 1);
            }

            void FreePages(physaddr_t pages, uint32_t pageCount)
            {
                MemoryRegion* currentRegion = freeList;
                while (currentRegion != nullptr)
                {
                    if (TryFreePagesInRegion(currentRegion, pages, pageCount))
                    {
#ifdef ECHIS_PMM_DEBUG_LOG_ALLOC
                        driver::debugcon::WriteFormatted("[PMM] Freeing %d physical pages at address %p\n", pageCount, pages);
#endif // ECHIS_PMM_DEBUG_LOG_ALLOC
                        return;
                    }

                    currentRegion = currentRegion->nextRegion;
                }

                return;
            }


            uint64_t GetTotalMemory()
            {
                uint64_t totalMemory = 0;

                MemoryRegion* current = freeList;
                while (current)
                {
                    totalMemory += current->totalSize;
                    current = current->nextRegion;
                }

                return totalMemory;
            }

            uint64_t GetTotalFreeMemory()
            {
                uint64_t totalFreeMemory = 0;

                MemoryRegion* current = freeList;
                while (current)
                {
                    totalFreeMemory += current->totalFree;
                    current = current->nextRegion;
                }

                return totalFreeMemory;
            }
        }
    }
}