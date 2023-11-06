#include <driver/framebuffer.h>

#include <driver/impl/framebuffer.h>

namespace echis
{
    namespace framebuffer
    {
        void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert)
        {
            framebufferImpl::Init(base, pitch, horiz, vert);
        }

        void PutPixel(uint32_t x, uint32_t y, uint32_t color)
        {
            framebufferImpl::PutPixel(x, y, color);
        }

        void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background)
        {
            framebufferImpl::DrawBitmap(bitmap, x, y, sizeX, sizeY, foreground, background);
        }
    }
}