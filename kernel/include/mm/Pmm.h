#ifndef VIPER_OS_MM_PMM_H
#define VIPER_OS_MM_PMM_H 1

#include <stdint.h>

namespace PMM
{
    constexpr int PAGE_SIZE = 0x1000;
    uint64_t VirtToPhys(uint64_t addr);
    uint64_t PhysToVirt(uint64_t addr);
    uint32_t NPAGES(uint64_t n);

    using PhysicalAddress = uint64_t;

    void Init();

    PhysicalAddress GetPage();
    void            FreePage(void* address);

    PhysicalAddress GetPages(uint32_t npages);
    void            FreePages(void* address, uint32_t npages);
}

#endif // VIPER_OS_MM_PMM_H