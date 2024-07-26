#ifndef VIPEROS_ECHIS_DRIVER_DEBUGCON_H
#define VIPEROS_ECHIS_DRIVER_DEBUGCON_H 1

#include <cstddef>

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void WriteString(const char* data, std::size_t size);
            void Write(const char* data);
        }
    }
}

#endif // VIPEROS_ECHIS_DRIVER_DEBUGCON_H