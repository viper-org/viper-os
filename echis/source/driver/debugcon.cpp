#include <driver/debugcon.h>

#include <atheris/driver/debugcon.h>

namespace echis
{
    namespace driver
    {
        namespace debugcon
        {
            void WriteString(const char* data, std::size_t size)
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    atheris::driver::debugcon::WriteChar(data[i]);
                }
            }

            void Write(const char* data)
            {
                std::size_t i = 0;
                while (data[i])
                {
                    atheris::driver::debugcon::WriteChar(data[i++]);
                }
            }
        }
    }
}