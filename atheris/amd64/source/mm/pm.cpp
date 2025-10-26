#include <mm/pm.h>

#include <echis/mm/physical.h>

#include <limine.h>

namespace atheris
{
    namespace amd64
    {
        namespace pm
        {
            static volatile limine_memmap_request MemmapRequest = {
                .id = LIMINE_MEMMAP_REQUEST,
                .revision = 1,
                .response = nullptr
            };

            void Initialize()
            {
                for (uint64_t i = 0; i < MemmapRequest.response->entry_count; ++i)
                {
                    auto entry = MemmapRequest.response->entries[i];

                    if (entry->type == LIMINE_MEMMAP_USABLE)
                    {
                        echis::mm::physical::AddMemoryRegion(entry->base, entry->length);
                    }
                }
            }

            limine_memmap_response* GetMemoryMap()
            {
                return MemmapRequest.response;
            }
        }
    }
}