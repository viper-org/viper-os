#ifndef VIPEROS_ECHIS_MM_PMM_H
#define VIPEROS_ECHIS_MM_PMM_H 1

#include <echis/mm/physaddr.h>

#include <stdint.h>

namespace echis
{
    namespace pmm
    {
        struct MemoryRegion
        {
            MemoryRegion(uint32_t size);

            uint8_t* bitmap;
            uint8_t* bitmapEnd;

            physaddr base;
            physaddr lastAllocatedPage;

            MemoryRegion* next;

            uint32_t totalSize;
            uint32_t totalFree;
        };
        
        /*
         * Set the page size to newPageSize
         *
         * Will only set once, any further calls will
         * leave the page size unchanged
         */
        void SetPageSize(int newPageSize);

        /*
         * Get the page size
         *
         * Returns zero before set
         */
        int GetPageSize();


        /*
         * Add a memory region to the allocator
         *
         * size must be at least 2 * pageSize
         */
        void AddRegion(void* base, uint32_t size);


        /*
         * Returns one physical page
         *
         * Returns zero if no pages are available
         */
        physaddr GetPage();

        /*
         * Returns pageCount physical pages
         *
         * Returns zero if no pages are available
         */
        physaddr GetPages(uint32_t pageCount);


        /*
         * Frees one physical page
         *
         * Does nothing if an invalid page is passed in
         */
        void FreePage(physaddr page);

        /*
         * Frees pageCount physical pages
         *
         * Does nothing if an invalid page is passed in
         */
        void FreePages(physaddr page, uint32_t pageCount);


        /*
         * Returns the total memory in bytes
         */
        uint64_t GetTotalMemory();

        /*
         * Returns the total free memory in bytes
         */
        uint64_t GetTotalFreeMemory();


#ifdef VIPEROS_ENABLE_TEST_FUNCTIONS
        /*
         * Resets the global state to startup values
         *
         * page size is set to zero
         */
        void ResetGlobalState();
#endif
    }
}

#endif // VIPEROS_ECHIS_MM_PMM_H