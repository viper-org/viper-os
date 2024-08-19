#ifndef VIPEROS_ECHIS_DRIVER_DEBUGCON_H
#define VIPEROS_ECHIS_DRIVER_DEBUGCON_H 1

#include <echis/driver/export.h>

#include <cstddef>

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void WriteString(const char* data, std::size_t size);
            void Write(const char* data);

            void WriteFormatted(const char* format, ...);

            DRIVER_EXPORT void KeDebugLog(const char* data);
            DRIVER_EXPORT void KeDebugLogFmt(const char* format, ...);
        }
    }
}

#endif // VIPEROS_ECHIS_DRIVER_DEBUGCON_H