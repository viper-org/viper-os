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

            void WriteString(const char* data, std::size_t size)
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    WriteChar(data[i]);
                }
            }

            void Write(const char* data)
            {
                std::size_t i = 0;
                while(data[i])
                {
                    WriteChar(data[i++]);
                }
            }
        }
    }
}