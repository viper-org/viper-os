#ifndef VIPER_OS_MM_PMM_H
#define VIPER_OS_MM_PMM_H 1
#include <stdint.h>

namespace PMM
{
    void Init();

    void* GetPage();
    void  FreePage(void* address);

    void* GetPages(uint32_t npages);
    void  FreePages(void* address, uint32_t npages);
}

#endif // VIPER_OS_MM_PMM_H