#include <driver/console.h>

#include <driver/impl/console.h>

namespace echis
{
    namespace console
    {
        void Init(FontInfo font, uint32_t screenHoriz, uint32_t screenVert)
        {
            consoleImpl::Init({font.bitmapData, font.sizeX, font.sizeY}, screenHoriz, screenVert);
        }

        void PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background)
        {
            consoleImpl::PutCharAt(c, x, y, foreground, background);
        }

        void PutChar(unsigned char c, uint32_t foreground, uint32_t background)
        {
            consoleImpl::PutChar(c, foreground, background);
        }

        void PutString(const char* data, size_t size, uint32_t foreground, uint32_t background)
        {
            consoleImpl::PutString(data, size, foreground, background);
        }

        void Print(const char* string, uint32_t foreground, uint32_t background)
        {
            consoleImpl::Print(string, foreground, background);
        }
    }
}