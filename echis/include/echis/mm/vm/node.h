#ifndef VIPEROS_ECHIS_MM_VM_NODE_H
#define VIPEROS_ECHIS_MM_VM_NODE_H 1

#include <cstdint>

namespace echis
{
    namespace vm
    {
        struct VMAllocNode
        {
            std::uint32_t numPages;
            std::uint64_t base;
        };
    }
}

#endif // VIPEROS_ECHIS_MM_VM_NODE_H