#ifndef VIPEROS_ECHIS_DRIVER_CONSOLE_IMPL_H
#define VIPEROS_ECHIS_DRIVER_CONSOLE_IMPL_H 1

#include <echis/driver/impl/framebuffer.h>

#include <stddef.h>
#include <stdint.h>

namespace echis
{
    struct consoleImpl
    {
        struct FontInfo
        {
            void* bitmapData { nullptr };
            uint32_t sizeX { 0 };
            uint32_t sizeY { 0 };
        };

        static void Init(FontInfo newFontInfo, uint32_t screenHoriz, uint32_t screenVert, FontInfo& fontInfo = globalFontInfo);

        template <typename FramebufferT = framebufferImpl>
        static void PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background)
        {
            FontInfo font = globalFontInfo;

            FramebufferT::DrawBitmap(reinterpret_cast<uint8_t*>(font.bitmapData) + (c * font.sizeX),
                          x * font.sizeX, y * font.sizeY,
                          font.sizeX, font.sizeY,
                          foreground, background);
        }

        template <typename ConsoleT = consoleImpl>
        static void PutChar(unsigned char c, uint32_t foreground, uint32_t background)
        {
            switch(c)
            {
                case '\n':
                    mX = 0;
                    ++mY;
                    break;
                
                default:
                    ConsoleT::PutCharAt(c, mX, mY, foreground, background);
                    ConsoleT::PutCharMoveCursor();
                    break;
            }
        }

        template <typename ConsoleT = consoleImpl>
        static void PutString(const char* data, size_t size, uint32_t foreground, uint32_t background)
        {
            for (size_t i = 0; i < size; ++i)
            {
                ConsoleT::PutChar(data[i], foreground, background);
            }
        }

        template <typename ConsoleT = consoleImpl>
        static void Print(const char* string, uint32_t foreground, uint32_t background)
        {
            for (size_t i = 0; string[i] != '\0'; ++i)
            {
                ConsoleT::PutChar(string[i], foreground, background);
            }
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