#ifndef VIPER_OS_MM_VMM_H
#define VIPER_OS_MM_VMM_H 1

#include <mm/Paging.h>

#include <stdint.h>

namespace vm
{
    void Init();

    void* GetPages(paging::AddressSpace* addrspace, uint32_t npages, uint16_t flags);
}

#endif // VIPER_OS_MM_VMM_H