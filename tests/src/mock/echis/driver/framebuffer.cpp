#include <mock/echis/driver/framebuffer.h>

namespace mock
{
    namespace echis
    {
        std::vector<framebuffer::DrawBitmapInvocation> framebuffer::DrawBitmapInvocations;

        void framebuffer::DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background)
        {
            DrawBitmapInvocations.push_back({
                bitmap, x, y, sizeX, sizeY, foreground, background
            });
        }
    }
}