// Copyright 2025 solar-mist

#include <echis/driver/debugcon.h>

#include <atheris/driver/debugcon.h>

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void Write(const char* data, unsigned long size)
            {
                while (size--)
                {
                    atheris::driver::debugcon::WriteChar(*data++);
                }
            }

            void WriteString(const char* data)
            {
                while (*data)
                {
                    atheris::driver::debugcon::WriteChar(*data++);
                }
            }
        }
    }
}