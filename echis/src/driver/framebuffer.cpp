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

            address[index + 0] = (color)       & 0xff;
            address[index + 1] = (color >> 8)  & 0xff;
            address[index + 2] = (color >> 16) & 0xff;
        }

        void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background)
        {
            uint8_t* address = reinterpret_cast<uint8_t*>(screen.base);
            uint32_t index = x * 4 + y * screen.pitch;

            uint8_t* bitmapData = reinterpret_cast<uint8_t*>(bitmap);

            for (uint32_t i = 0; i < sizeY; ++i)
            {
                uint8_t row = bitmapData[i];

                for (uint32_t j = 0; j < sizeX; j++)
                {
                    uint32_t color = (row >> (sizeX - j - 1) & 0x1) ? foreground : background;

                    address[index]     =  color        & 0xff;
                    address[index + 1] = (color >> 8)  & 0xff;
                    address[index + 2] = (color >> 16) & 0xff;

                    index += 4;
                }

                index += screen.pitch;
                index -= sizeX * 4;
            }
        }
    }
}