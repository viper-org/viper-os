#include <echis/mm/pmm.h>

#include <catch2/catch_test_macros.hpp>

#include <memory>

namespace pmmTests
{
    class PMMTestsFixture
    {
    public:
        PMMTestsFixture()
        {
            echis::pmm::ResetGlobalState();
        }
    };

    constexpr int pageSize = 0x1000;
    
    TEST_CASE_METHOD(PMMTestsFixture, "Setting page size", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);
        REQUIRE(echis::pmm::GetPageSize() == pageSize);

        echis::pmm::SetPageSize(pageSize + 1); // Any value other than the previously set size
        REQUIRE(echis::pmm::GetPageSize() == pageSize); // Page size should not change once set
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Adding one memory region", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);

        // Calculate actual size of region after removing bookkeeping
        uint32_t bitmapSize = (regionSize - sizeof(echis::pmm::MemoryRegion)) / (pageSize * CHAR_BIT);
        uint32_t expectedSize = regionSize - bitmapSize - sizeof(echis::pmm::MemoryRegion);

        REQUIRE(echis::pmm::GetTotalMemory()     == expectedSize);
        REQUIRE(echis::pmm::GetTotalFreeMemory() == expectedSize); // Free memory should be the same as total memory here
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Adding multiple memory regions", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        constexpr int regionCount = 3;
        
        std::array<std::unique_ptr<std::byte[]>, regionCount> regions;
        for (auto& region : regions)
        {
            region = std::make_unique<std::byte[]>(regionSize);
            echis::pmm::AddRegion(region.get(), regionSize);
        }

        // Calculate actual size of region after removing bookkeeping
        uint32_t bitmapSize = (regionSize - sizeof(echis::pmm::MemoryRegion)) / (pageSize * CHAR_BIT);
        uint32_t expectedSize = (regionSize - bitmapSize - sizeof(echis::pmm::MemoryRegion)) * regionCount;

        REQUIRE(echis::pmm::GetTotalMemory()     == expectedSize);
        REQUIRE(echis::pmm::GetTotalFreeMemory() == expectedSize); // Free memory should be the same as total memory here
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Allocating one page", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);
        const uint64_t regionEnd = reinterpret_cast<uint64_t>(region.get()) + regionSize;

        echis::pmm::physaddr allocatedPage = echis::pmm::GetPage();

        // Allocated page should be inside of the region given to the PMM
        REQUIRE(allocatedPage >= reinterpret_cast<echis::pmm::physaddr>(region.get()));
        REQUIRE(allocatedPage <= reinterpret_cast<echis::pmm::physaddr>(regionEnd - pageSize));

        // Free memory should be exactly one page less than total memory
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory() - pageSize);
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Consecutive single page allocations", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);
        const uint64_t regionEnd = reinterpret_cast<uint64_t>(region.get()) + regionSize;

        constexpr int allocationCount = 2;

        for (int i = 0; i < allocationCount; ++i)
        {
            echis::pmm::physaddr allocatedPage = echis::pmm::GetPage();

            // Allocated page should be inside of the region given to the PMM
            REQUIRE(allocatedPage >= reinterpret_cast<echis::pmm::physaddr>(region.get()));
            REQUIRE(allocatedPage <= reinterpret_cast<echis::pmm::physaddr>(regionEnd - pageSize));

            // Free memory should be exactly i pages less than total memory
            REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory() - (pageSize * (i + 1)));
        }
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Allocating multiple pages", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);
        const uint64_t regionEnd = reinterpret_cast<uint64_t>(region.get()) + regionSize;

        constexpr int pageCount = 2;

        echis::pmm::physaddr allocatedPages = echis::pmm::GetPages(pageCount);

        // Allocated pages should be inside of the region given to the PMM
        REQUIRE(allocatedPages >= reinterpret_cast<echis::pmm::physaddr>(region.get()));
        REQUIRE(allocatedPages <= reinterpret_cast<echis::pmm::physaddr>(regionEnd - pageCount * pageSize));

        // Free memory should be exactly pageCount pages less than total memory
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory() - pageCount * pageSize);
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Allocating more pages than available", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);
        const uint64_t regionEnd = reinterpret_cast<uint64_t>(region.get()) + regionSize;

        constexpr int pageCount = regionSize / pageSize + 1;

        echis::pmm::physaddr allocatedPages = echis::pmm::GetPages(pageCount);

        // Allocated pages should be null as there are not enough pages to allocate
        REQUIRE(allocatedPages == 0);

        // Free memory should be unchanged
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory());
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Allocating zero pages", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        echis::pmm::physaddr allocatedPage = echis::pmm::GetPages(0);

        // No pages should be allocated
        REQUIRE(allocatedPage == 0);

        // Free memory should be unchanged
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory());
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Freeing one page", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);

        echis::pmm::physaddr allocatedPage = echis::pmm::GetPage();

        echis::pmm::FreePage(allocatedPage);

        // Free memory should be the same as total memory now
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory());
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Freeing multiple pages", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);

        echis::pmm::physaddr allocatedPages = echis::pmm::GetPages(2);

        echis::pmm::FreePages(allocatedPages, 2);

        // Free memory should be the same as total memory now
        REQUIRE(echis::pmm::GetTotalFreeMemory() == echis::pmm::GetTotalMemory());
    }

    TEST_CASE_METHOD(PMMTestsFixture, "Freeing invalid page", "[pmmTests]")
    {
        echis::pmm::SetPageSize(pageSize);

        constexpr int regionSize = pageSize * 10;
        std::unique_ptr<std::byte[]> region = std::make_unique<std::byte[]>(regionSize);
        echis::pmm::AddRegion(region.get(), regionSize);

        uint64_t freeMemory = echis::pmm::GetTotalFreeMemory();

        echis::pmm::FreePage(0);

        // Free memory should be unchanged after freeing an invalid page
        REQUIRE(echis::pmm::GetTotalFreeMemory() == freeMemory);
    }
}