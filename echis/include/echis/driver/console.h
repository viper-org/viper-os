#ifndef VIPEROS_ECHIS_DRIVER_CONSOLE_H
#define VIPEROS_ECHIS_DRIVER_CONSOLE_H 1

#include <stddef.h>
#include <stdint.h>

namespace echis
{
    namespace console
    {
        struct FontInfo
        {
            void* bitmapData { nullptr };
            uint32_t sizeX { 0 };
            uint32_t sizeY { 0 };
        };

        /*
         * Initialize the console with a
         * specified font
         *
         * May only be called once, further
         * calls will be ignored
         */
        void Init(FontInfo font, uint32_t screenHoriz, uint32_t screenVert);


        /*
         * Puts a character at a specified location
         */
        void PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background);

        /*
         * Puts a character at the next location
         */
        void PutChar(unsigned char c, uint32_t foreground, uint32_t background);

        /*
         * Prints an array of characters
         * with specified size
         *
         * Size must be equal to or less than
         * the size of the array
         */
        void PutString(const char* data, size_t size, uint32_t foreground, uint32_t background);

        /*
         * Prints a string
         *
         * The string must end with a null
         * terminator
         */
        void Print(const char* data, uint32_t foreground, uint32_t background);
    }
}

#endif