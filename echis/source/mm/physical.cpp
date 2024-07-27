#include <mm/physical.h>

#include <util/math.h>
#include <util/debug.h>

#if defined(ECHIS_PMM_DEBUG_LOG_ALLOC) || defined(ECHIS_PMM_DEBUG_LOG_FREE)
#include <driver/debugcon.h>
#endif

#include <atheris/mm/vm.h>

#include <climits>
#include <cstring>
#include <new>

namespace echis
{
    namespace mm
    {
        namespace physical
        {
            static unsigned int PageSize = 0;
            static MemoryRegion* memoryRegionList = nullptr;
            // TODO: Add mutex

            static inline void AppendMemoryRegion(MemoryRegion* region)
            {
                if (!memoryRegionList)
                {
                    memoryRegionList = region;
                }
                else
                {
                    region->next = memoryRegionList;
                    memoryRegionList = region;
                }
            }

            MemoryRegion::MemoryRegion(std::uint32_t size)
                : lastAllocatedPage(0)
                , next(nullptr)
                , totalSize(size)
            {
            }

            void SetPageSize(unsigned int newPageSize)
            {
                PageSize = newPageSize;
            }

            unsigned int GetPageSize()
            {
                return PageSize;
            }

            void AddMemoryRegion(physaddr base, std::uint32_t size)
            {
                void* virtualBase = atheris::vm::GetVirtualAddress(base);

                MemoryRegion* region = new(virtualBase) MemoryRegion(size);

                std::uint32_t bitmapSize = (size - sizeof(MemoryRegion)) / (PageSize * CHAR_BIT);
                char* bitmapStartAddr = static_cast<char*>(virtualBase) + sizeof(MemoryRegion);
                region->bitmapStart = new(bitmapStartAddr) std::uint8_t[bitmapSize];
                region->bitmapEnd = region->bitmapStart + bitmapSize;

                region->base = util::AlignUp(base + sizeof(MemoryRegion) + bitmapSize, PageSize);
                region->totalSize -= bitmapSize + sizeof(MemoryRegion);
                region->totalFree = region->totalSize;

                std::memset(region->bitmapStart, 0xFF, bitmapSize);

                AppendMemoryRegion(region);
            }

            static inline bool BitmapTest(MemoryRegion* region, physaddr address, std::uint32_t pageCount)
            {
                bool result = true;
                std::uint64_t index = address - region->base;

                std::uint64_t endIndex = (index + (pageCount * PageSize - 1)) / (PageSize * CHAR_BIT);
                if (&region->bitmapStart[endIndex] >= region->bitmapEnd)
                {
                    return false;
                }

                for (std::uint64_t i = index; i < index + (pageCount * PageSize); i += PageSize)
                {
                    result = region->bitmapStart[i / (PageSize * CHAR_BIT)] & (1 << ((i / PageSize) % CHAR_BIT));
                    if (!result) break;
                }

                return result;
            }

            static inline void BitmapSet(MemoryRegion* region, physaddr address, std::uint32_t pageCount)
            {
                std::uint64_t index = address - region->base;

                for (std::uint64_t i = index; i < index + (pageCount * PageSize); i += PageSize)
                {
                    region->bitmapStart[i / (PageSize * CHAR_BIT)] &= ~(1 << ((i / PageSize) % CHAR_BIT));
                }
            }

            
            bool TryAllocateAt(MemoryRegion* region, physaddr address, std::uint32_t pageCount)
            {
                if (!BitmapTest(region, address, pageCount))
                {
                    return false;
                }

                BitmapSet(region, address, pageCount);
                region->totalFree -= pageCount * PageSize;
                region->lastAllocatedPage = address + pageCount * PageSize;
                return true;
            }

            physaddr CheckRegionFrom(MemoryRegion* region, physaddr start, std::uint32_t pageCount)
            {
                const physaddr regionEnd = region->base + region->totalSize;

                for (physaddr address = start; address < regionEnd; address += PageSize)
                {
                    if (TryAllocateAt(region, address, pageCount))
                    {
                        return address;
                    }
                }

                return 0;
            }

            physaddr CheckRegion(MemoryRegion* region, std::uint32_t pageCount)
            {
                if (region->lastAllocatedPage != 0)
                {
                    if (physaddr address = CheckRegionFrom(region, region->lastAllocatedPage, pageCount))
                    {
                        return address;
                    }
                }
                if (physaddr address = CheckRegionFrom(region, region->base, pageCount))
                {
                    return address;
                }

                return 0;
            }

            physaddr GetPage()
            {
                return GetPages(1);
            }
            physaddr GetPages(std::uint32_t pageCount)
            {
                if (pageCount == 0)
                {
                    return 0;
                }

                MemoryRegion* currentRegion = memoryRegionList;
                while (currentRegion != nullptr)
                {
                    if (physaddr address = CheckRegion(currentRegion, pageCount))
                    {
#ifdef ECHIS_PMM_DEBUG_LOG_ALLOC
                        driver::debugcon::WriteFormatted("[PMM] Allocating %d physical pages at address %p\n", pageCount, address);
#endif // ECHIS_PMM_DEBUG_LOG_ALLOC
                        return address;
                    }

                    currentRegion = currentRegion->next;
                }

                return 0; // No pages found
            }


            void FreePageInRegion(MemoryRegion* region, physaddr page)
            {
                if (!BitmapTest(region, page, 1))
                {
                    region->totalFree += PageSize;
                }

                std::uint64_t index = page - region->base;
                region->bitmapStart[index / (PageSize / CHAR_BIT)] |= ((index / PageSize) % CHAR_BIT);
            }

            bool TryFreePagesInRegion(MemoryRegion* region, physaddr pages, std::uint32_t pageCount)
            {
                physaddr pagesEnd = pages + (pageCount * PageSize);
                physaddr regionEnd = region->base + region->totalSize;

                if (pages >= region->base && pagesEnd <= regionEnd)
                {
                    for (physaddr page = pages; page < pagesEnd; page += PageSize)
                    {
                        FreePageInRegion(region, page);
                    }
                    return true;
                }

                return false;
            }

            void FreePage(physaddr page)
            {
                FreePages(page, 1);
            }

            void FreePages(physaddr pages, std::uint32_t pageCount)
            {
                MemoryRegion* currentRegion = memoryRegionList;
                while (currentRegion != nullptr)
                {
                    if (TryFreePagesInRegion(currentRegion, pages, pageCount))
                    {
#ifdef ECHIS_PMM_DEBUG_LOG_ALLOC
                        driver::debugcon::WriteFormatted("[PMM] Freeing %d physical pages at address %p\n", pageCount, pages);
#endif // ECHIS_PMM_DEBUG_LOG_ALLOC
                        return;
                    }

                    currentRegion = currentRegion->next;
                }

                return;
            }


            std::uint64_t GetTotalMemory()
            {
                std::uint64_t totalMemory = 0;

                MemoryRegion* current = memoryRegionList;
                while (current)
                {
                    totalMemory += current->totalSize;
                    current = current->next;
                }

                return totalMemory;
            }

            std::uint64_t GetTotalFreeMemory()
            {
                std::uint64_t totalFreeMemory = 0;

                MemoryRegion* current = memoryRegionList;
                while (current)
                {
                    totalFreeMemory += current->totalFree;
                    current = current->next;
                }

                return totalFreeMemory;
            }
        }
    }
}