#ifndef VIPEROS_ATHERIS_PUBLIC_ATHERIS_DRIVER_DEBUGCON_H
#define VIPEROS_ATHERIS_PUBLIC_ATHERIS_DRIVER_DEBUGCON_H 1

#include <cstddef>

namespace atheris
{
    namespace driver
    {
        namespace debugcon
        {
            void WriteChar(char c);
            void WriteString(const char* data, std::size_t size);
            void Write(const char* data);
        }
    }
}

#endif // VIPEROS_ATHERIS_PUBLIC_ATHERIS_DRIVER_DEBUGCON_H