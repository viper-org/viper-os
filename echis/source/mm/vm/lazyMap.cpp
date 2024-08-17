#include <mm/vm/lazyMap.h>

#include <mm/physical.h>

#include <atheris/private/mm/vm.h>

#include <algorithm>

namespace echis
{
    namespace vm
    {
        bool ShouldLazyMap(std::uint64_t address)
        {
            atheris::vm::AddressSpace* active = atheris::vm::AddressSpace::Active();

            auto it = std::find_if(active->allocatedRegions.begin(), active->allocatedRegions.end(), [address](auto region){
                return address >= region.base && address <= region.base + region.numPages * mm::physical::GetPageSize();
            });

            return it != active->allocatedRegions.end();
        }
    }
}