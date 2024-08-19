#ifndef VIPEROS_ECHIS_MM_VM_NODE_H
#define VIPEROS_ECHIS_MM_VM_NODE_H 1

#include <cstdint>

namespace echis
{
    namespace vm
    {
        struct VMAllocNode
        {
            std::uint64_t numPages;
            std::uint64_t base;
        };

        namespace flags
        {
            enum flags : std::uint16_t
            {
                LazyMapping
            };
        }

        struct VMAllocatedRegion
        {
            std::uint64_t numPages;
            std::uint64_t base;

            std::uint16_t flags;
            std::uint16_t mappingFlags;
        };
    }
}

#endif // VIPEROS_ECHIS_MM_VM_NODE_H