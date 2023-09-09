#ifndef VIPER_OS_DRIVERS_FRAMEBUFFER_H
#define VIPER_OS_DRIVERS_FRAMEBUFFER_H 1

#include <stdint.h>

namespace Framebuffer
{
    struct FB
    {
        void* base;
        uint64_t pitch;
        uint64_t horiz;
        uint64_t vert;
    };

    class Color
    {
    public:
        Color(unsigned char r, unsigned char g, unsigned char b);
        Color(unsigned int hex);

        operator unsigned int();

        unsigned char r() const;
        unsigned char g() const;
        unsigned char b() const;
    private:
        unsigned char m_R, m_G, m_B;
    };

    void Init();

    void PutPixel(unsigned int x, unsigned int y, Color color);
    void DrawBitmap(void* bitmap, uint32_t sizeX, uint32_t sizeY, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);
}

#endif // VIPER_OS_DRIVERS_FRAMEBUFFER_H