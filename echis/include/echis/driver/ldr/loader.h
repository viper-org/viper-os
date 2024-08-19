#ifndef VIPEROS_ECHIS_DRIVER_LDR_LOADER_H
#define VIPEROS_ECHIS_DRIVER_LDR_LOADER_H 1

#include <cstdint>
#include <cstddef>

namespace echis
{
    namespace driver
    {
        namespace ldr
        {
            struct [[gnu::packed]] DriverHeader
            {
                void*(*kernel_func_getter)(const char*); // gets an arbitrary kernel function by its name

                std::ssize_t (*read)(void* buf, std::size_t count);
                std::ssize_t (*write)(const void* buf, std::size_t count);
                // TODO: Add mmap and other VFS functions

                const char* name; // file will exist at /dev/name
            };

            struct Driver
            {
                DriverHeader* header;
                void(*init)();
            };

            void Init();
            Driver LoadDriver(void* addr);
        }
    }
}

#endif // VIPEROS_ECHIS_DRIVER_LDR_LOADER_H