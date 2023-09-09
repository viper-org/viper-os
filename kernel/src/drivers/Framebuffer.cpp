#include <drivers/Framebuffer.h>
#include <limine.h>

namespace Framebuffer
{
    Color::Color(unsigned char r, unsigned char g, unsigned char b)
        :m_R(r), m_G(g), m_B(b)
    {
    }

    Color::Color(unsigned int hex)
    {
        m_R = (hex >> 16) & 0xFF;
        m_G = (hex >> 8)  & 0xFF;
        m_B = hex & 0xFF;
    }

    Color::operator unsigned int()
    {
        return (m_R << 16) + (m_G << 8) + m_B;
    }

    unsigned char Color::r() const
    {
        return m_R;
    }
    unsigned char Color::g() const
    {
        return m_G;
    }
    unsigned char Color::b() const
    {
        return m_B;
    }

    static volatile limine_framebuffer_request framebufferRequest = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 1,
        .response = nullptr,
    };

    FB framebuffer;

    void Init()
    {
        framebuffer = {
            framebufferRequest.response->framebuffers[0]->address, framebufferRequest.response->framebuffers[0]->pitch,
            framebufferRequest.response->framebuffers[0]->width, framebufferRequest.response->framebuffers[0]->height,
        };
    }
    
    void PutPixel(unsigned int x, unsigned int y, Color color)
    {
        unsigned char* screen = (unsigned char*)framebuffer.base;
        unsigned int where = x * 4 + y * framebuffer.pitch;
        screen[where] = color.r();
        screen[where + 1] = color.g();
        screen[where + 2] = color.b();
    }

    void DrawBitmap(void* bitmap, uint32_t sizeX, uint32_t sizeY, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
    {
        uint8_t* screen = (uint8_t*)framebuffer.base;
        uint32_t where = x * 4 + y * framebuffer.pitch;

        uint8_t* bitmapData = (uint8_t*)bitmap;

        for(uint32_t i = 0; i < sizeY; i++)
        {
            uint8_t data = bitmapData[i];
            
            for(uint32_t j = 0; j < sizeX; j++)
            {
                uint32_t color = (data >> (sizeX - j) & 0x1) ? fg : bg;

                screen[where]     =  color         & 0xFF;
                screen[where + 1] = (color >> 8)   & 0xFF;
                screen[where + 2] = (color >> 16)  & 0xFF;
                
                where += 4;
            }
            where += framebuffer.pitch;
            where -= sizeX * 4;
        }
    }
}