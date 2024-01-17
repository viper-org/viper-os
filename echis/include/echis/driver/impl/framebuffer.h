#ifndef VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_IMPL_H
#define VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_IMPL_H 1

#ifdef MOCK_VIPEROS_ECHIS_DRIVER_FRAMEBUFFER_H
#include <mock/echis/driver/framebuffer.h>
#else

#include <stdint.h>

namespace echis
{
    struct framebufferImpl
    {
        static void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert);

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

        static void*    GetBase();
        static uint64_t GetHoriz();
        static uint64_t GetVert();
        static uint64_t GetPitch();
    };
}

#endif

#endif