#include <iostream>
#ifndef VIPEROS_ECHIS_DRIVER_CONSOLE_H
#define VIPEROS_ECHIS_DRIVER_CONSOLE_H 1

#include <echis/driver/framebuffer.h>

#include <stdint.h>

namespace echis
{
    struct console
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
        static void Init(FontInfo newFontInfo, uint32_t screenHoriz, uint32_t screenVert, FontInfo& fontInfo = globalFontInfo);


        /*
         * Puts a character at a specified location
         */
        template <typename T = framebuffer>
        static void PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background)
        {
            FontInfo font = globalFontInfo;

            T::DrawBitmap(reinterpret_cast<uint8_t*>(font.bitmapData) + (c * font.sizeX),
                          x * font.sizeX, y * font.sizeY,
                          font.sizeX, font.sizeY,
                          foreground, background);
        }

        /*
         * Puts a character at the next location
         */
        template <typename T = console>
        static void PutChar(unsigned char c, uint32_t foreground, uint32_t background)
        {
            T::PutCharAt(c, mX, mY, foreground, background);

            T::PutCharMoveCursor();
        }

#ifdef VIPEROS_ENABLE_TEST_FUNCTIONS
        /*
         * Resets the global state to startup values
         *
         * FontInfo is reset to an unspecified value
         */
        static void ResetGlobalState();
#endif


    private:
        static void PutCharMoveCursor()
        {
            if (++mX >= mHoriz / globalFontInfo.sizeX)
            {
                mX = 0;
                mY += 1;
            }
        }

        static FontInfo globalFontInfo;

        static uint32_t mX;
        static uint32_t mY;
        static uint32_t mHoriz;
        static uint32_t mVert;
    };
}

#endif