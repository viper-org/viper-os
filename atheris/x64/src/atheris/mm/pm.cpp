#include <atheris/private/mm/pm.h>

#include <echis/mm/pmm.h>

#include <limine.h>

namespace atheris
{
    namespace pm
    {
        static volatile limine_memmap_request memmapRequest = {
            .id = LIMINE_MEMMAP_REQUEST,
            .revision = 1,
            .response = nullptr,
        };

        limine_memmap_response* memoryMap;

        void Init()
        {
            echis::pmm::SetPageSize(0x1000);
            for (uint64_t i = 0; i < memmapRequest.response->entry_count; ++i)
            {
                auto entry = memmapRequest.response->entries[i];
                
                if (entry->type == LIMINE_MEMMAP_USABLE)
                {
                    echis::pmm::AddRegion(reinterpret_cast<void*>(entry->base), entry->length);
                }
            }

            memoryMap = memmapRequest.response;
        }
    }
}