// Copyright 2025 solar-mist

#include <atheris/driver/debugcon.h>

#include <driver/port.h>

namespace atheris
{
    namespace driver
    {
        namespace debugcon
        {
            constexpr amd64::driver::Port<unsigned char> COM1(0x3F8);

            void WriteChar(char c)
            {
                COM1.write(c);
            }
        }
    }
}