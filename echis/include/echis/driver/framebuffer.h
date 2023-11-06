#ifndef VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H
#define VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H 1

#include <stdint.h>

namespace echis
{
    namespace framebuffer
    {
        /*
         * Initialize the framebuffer
         *
         * May only be called once, further
         * calls will be ignored
         */
        void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert);

        /*
         * Puts a pixel at a specific x and
         * y coordinate
         *
         * If the coordinates are outside
         * the bounds of the framebuffer,
         * the call is ignored
         */
        void PutPixel(uint32_t x, uint32_t y, uint32_t color);

        /*
         * Draws a bitmap of specified size
         * at specified x and y coordinates
         *
         * 1 bit will put the foreground
         * color, and a 0 bit will put
         * the background color
         */
        void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background);
    }
}

#endif