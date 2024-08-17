#ifndef VIPEROS_ECHIS_MM_VM_LAZY_MAP_H
#define VIPEROS_ECHIS_MM_VM_LAZY_MAP_H 1

#include <cstdint>

namespace echis
{
    namespace vm
    {
        bool ShouldLazyMap(std::uint64_t address);
    }
}

#endif // VIPEROS_ECHIS_MM_VM_LAZY_MAP_H