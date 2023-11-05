#include <driver/framebuffer.h>

namespace echis
{
    namespace framebuffer
    {
        struct Screen
        {
            void* base;
            uint64_t pitch;
            uint64_t horiz;
            uint64_t vert;
        } screen;

        void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert)
        {
            screen.base = base;
            screen.pitch = pitch;
            screen.horiz = horiz;
            screen.vert = vert;
        }

        void PutPixel(uint32_t x, uint32_t y, uint32_t color)
        {
            uint8_t* address = reinterpret_cast<uint8_t*>(screen.base);
            uint32_t index = x * 4 + y * screen.pitch;

            address[index]     = (color >> 16) & 0xff;
            address[index + 1] = (color >> 8)  & 0xff;
            address[index + 2] = (color)       & 0xff;
        }
    }
}