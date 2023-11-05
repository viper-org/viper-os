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
    }
}

#endif