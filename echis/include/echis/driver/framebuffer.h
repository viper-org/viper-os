#ifndef VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H
#define VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H 1

#ifdef MOCK_VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H
#include <mock/echis/driver/framebuffer.h>
#else

#include <stdint.h>

namespace echis
{
    struct framebuffer
    {
        /*
         * Initialize the framebuffer
         *
         * May only be called once, further
         * calls will be ignored
         */
        static void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert);

        /*
         * Puts a pixel at a specific x and
         * y coordinate
         *
         * If the coordinates are outside
         * the bounds of the framebuffer,
         * the call is ignored
         */
        static void PutPixel(uint32_t x, uint32_t y, uint32_t color);

        /*
         * Draws a bitmap of specified size
         * at specified x and y coordinates
         *
         * 1 bit will put the foreground
         * color, and a 0 bit will put
         * the background color
         */
        static void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background);

        static void test();
    };
}

#endif

#endif