// Copyright 2025 solar-mist

#ifndef VIPEROS_ECHIS_DRIVER_DEBUGCON_H
#define VIPEROS_ECHIS_DRIVER_DEBUGCON_H 1

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void Write(const char* data, unsigned long size);
            void WriteString(const char* data); // Null terminated string
        }
    }
}

#endif // VIPEROS_ECHIS_DRIVER_DEBUGCON_H