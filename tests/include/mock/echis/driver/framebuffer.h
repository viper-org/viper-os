#ifndef VIPEROS_MOCK_ECHIS_DRIVER_FRAMEBUFFER_H
#define VIPEROS_MOCK_ECHIS_DRIVER_FRAMEBUFFER_H 1

#include <stdint.h>
#include <iostream>
#include <vector>

namespace mock
{
    namespace echis
    {
        struct framebuffer
        {
            struct DrawBitmapInvocation
            {
                void* bitmap;
                uint32_t x;
                uint32_t y;
                uint32_t sizeX;
                uint32_t sizeY;
                uint32_t foreground;
                uint32_t background;
            };
            static std::vector<DrawBitmapInvocation> DrawBitmapInvocations;
            static void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background);
        };
    }
}

#endif // VIPEROS_MOCK_ECHIS_DRIVER_FRAMEBUFFER_H