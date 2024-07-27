#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_DRIVER_DEBUGCON_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_DRIVER_DEBUGCON_H 1

namespace atheris
{
    namespace driver
    {
        namespace debugcon
        {
            /*
             * Writes a single character to a display
             * or serial output
             */
            void WriteChar(char c);
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_DRIVER_DEBUGCON_H