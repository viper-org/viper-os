#include "drivers/Framebuffer.h"
#include "fs/Module.h"
#include <string.h>
#include <drivers/Terminal.h>

namespace Framebuffer
{
    extern struct FB framebuffer;
}

namespace Terminal
{
    static unsigned int col = 0;
    static unsigned int row = 0;
    
    static unsigned int maxCol;
    static unsigned int maxRow;

    static fs::Module font;

    void Init()
    {
        maxCol = Framebuffer::framebuffer.horiz;
        maxRow = Framebuffer::framebuffer.vert;

        font = fs::Module::Get("/font.bmp");
    }

    constexpr int FONT_SIZE = 8;

    void PutChar(char c, int x, int y, Framebuffer::Color fg, Framebuffer::Color bg)
    {
        Framebuffer::DrawBitmap((char*)font.getAddress() + (c * FONT_SIZE), 
                                FONT_SIZE, FONT_SIZE,
                                x * FONT_SIZE, y * FONT_SIZE,
                                fg, bg);
    }

    void PutChar(char c, Framebuffer::Color fg, Framebuffer::Color bg)
    {
        switch(c)
        {
            case '\n':
            {
                col = 0;
                row++;
                break;
            }
            default:
            {
                PutChar(c, col, row, fg, bg);
                if (++col == maxCol)
                {
                    col = 0;
                    row++;
                    // TODO: Scroll
                }
                break;
            }
        }
    }

    void PutString(const char* data, unsigned int size, Framebuffer::Color fg, Framebuffer::Color bg)
    {
        while (size--)
        {
            PutChar(*data, fg, bg);
            data++;
        }
    }

    void Print(const char* data, Framebuffer::Color fg, Framebuffer::Color bg)
    {
        while (*data)
        {
            PutChar(*data, fg, bg);
            data++;
        }
    }
}