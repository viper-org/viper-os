#include <mm/pmm.h>

#include <util/math.h>

#include <limits.h>
#include <string.h>
#include <new>

namespace echis
{
    namespace pmm
    {
        static int pageSize = 0;
        static MemoryRegion* memoryRegions = nullptr;


        MemoryRegion::MemoryRegion(uint32_t size)
            : next(nullptr)
            , totalSize(size)
        {
        }


        void SetPageSize(int newPageSize)
        {
            if (pageSize == 0)
            {
                pageSize = newPageSize;
            }
        }

        int GetPageSize()
        {
            return pageSize;
        }

        
        void AddRegion(void* base, uint32_t size)
        {
            MemoryRegion* region = new(base) MemoryRegion(size);

            uint32_t bitmapSize = (size - sizeof(MemoryRegion)) / (pageSize * CHAR_BIT);
            region->bitmap = new(static_cast<uint8_t*>(base) + sizeof(MemoryRegion)) uint8_t[bitmapSize];
            region->bitmapEnd = region->bitmap + bitmapSize;

            region->base = reinterpret_cast<physaddr>(util::AlignUp(region->bitmapEnd, pageSize));
            region->totalSize -= bitmapSize + sizeof(MemoryRegion);
            region->totalFree = region->totalSize;

            memset(region->bitmap, 0xFF, bitmapSize);

            if (!memoryRegions)
            {
                memoryRegions = region;
            }
            else
            {
                region->next = memoryRegions;
                memoryRegions = region;
            }
        }


        bool BitmapTest(MemoryRegion* region, physaddr address, uint32_t pageCount)
        {
            bool result = true;
            uint64_t index = address - region->base;

            uint64_t endIndex = (index + (pageCount * pageSize - 1)) / (pageSize * CHAR_BIT);
            if (&region->bitmap[endIndex] >= region->bitmapEnd)
            {
                return false;
            }

            for (uint64_t i = index; i < index + (pageCount * pageSize); i += pageSize)
            {
                result = region->bitmap[i / (pageSize * CHAR_BIT)] & (1 << ((i / pageSize) % CHAR_BIT));
                if (!result)
                {
                    break;
                }
            }

            return result;
        }

        void BitmapSet(MemoryRegion* region, physaddr address, uint32_t pageCount)
        {
            uint64_t index = address - region->base;

            for (uint64_t i = index; i < index + (pageCount * pageSize); i += pageSize)
            {
                region->bitmap[i / (pageSize * CHAR_BIT)] &= ~(1 << ((i / pageSize) % CHAR_BIT));
            }
        }

        bool TryAllocate(MemoryRegion* region, physaddr address, uint32_t pageCount)
        {
            if (!BitmapTest(region, address, pageCount))
            {
                return false;
            }

            BitmapSet(region, address, pageCount);
            region->totalFree -= pageCount * pageSize;
            region->lastAllocatedPage = address;
            return true;
        }

        physaddr CheckRegionFrom(MemoryRegion* region, physaddr start, uint32_t pageCount)
        {
            const physaddr regionEnd = region->base + region->totalSize;

            for (physaddr address = start; address < regionEnd; address += pageSize)
            {
                if (TryAllocate(region, address, pageCount))
                {
                    return address;
                }
            }

            return 0;
        }

        physaddr CheckRegion(MemoryRegion* region, uint32_t pageCount)
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

        physaddr GetPages(uint32_t pageCount)
        {
            if (pageCount == 0)
            {
                return 0;
            }

            MemoryRegion* currentRegion = memoryRegions;
            while (currentRegion != nullptr)
            {
                if (physaddr address = CheckRegion(currentRegion, pageCount))
                {
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
                region->totalFree += pageSize;
            }

            uint64_t index = page - region->base;
            region->bitmap[index / (pageSize / CHAR_BIT)] |= ((index / pageSize) % CHAR_BIT);
        }

        bool TryFreePagesInRegion(MemoryRegion* region, physaddr pages, uint32_t pageCount)
        {
            physaddr pagesEnd = pages + (pageCount * pageSize);
            physaddr regionEnd = region->base + region->totalSize;

            if (pages >= region->base && pagesEnd <= regionEnd)
            {
                for (physaddr page = pages; page < pagesEnd; page += pageSize)
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

        void FreePages(physaddr pages, uint32_t pageCount)
        {
            MemoryRegion* currentRegion = memoryRegions;
            while (currentRegion != nullptr)
            {
                if (TryFreePagesInRegion(currentRegion, pages, pageCount))
                {
                    return;
                }

                currentRegion = currentRegion->next;
            }

            return;
        }


        uint64_t GetTotalMemory()
        {
            uint64_t totalMemory = 0;

            MemoryRegion* current = memoryRegions;
            while(current != nullptr)
            {
                totalMemory += current->totalSize;

                current = current->next;
            }

            return totalMemory;
        }

        uint64_t GetTotalFreeMemory()
        {
            uint64_t totalFree = 0;

            MemoryRegion* current = memoryRegions;
            while(current != nullptr)
            {
                totalFree += current->totalFree;

                current = current->next;
            }

            return totalFree;
        }


#ifdef VIPEROS_ENABLE_TEST_FUNCTIONS
        void ResetGlobalState()
        {
            memoryRegions = nullptr;
            pageSize = 0;
        }
#endif
    }
}