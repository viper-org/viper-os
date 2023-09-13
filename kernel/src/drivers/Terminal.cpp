#include <drivers/Terminal.h>
#include <drivers/Framebuffer.h>

#include <fs/Module.h>
#include <fs/DevFs.h>

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

    void DriverWrite(const void* buffer, size_t count);

    void Init()
    {
        maxCol = Framebuffer::framebuffer.horiz / 8;
        maxRow = Framebuffer::framebuffer.vert / 8;

        font = fs::Module::Get("/font.bmp");

        fs::devfs::Filesystem::RegisterDeviceFile("tty", nullptr, DriverWrite);
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
        if (row >= maxRow)
        {
            asm("1: hlt; jmp 1b"); // TODO: Scroll or panic
        }
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


    void DriverWrite(const void* buffer, size_t count)
    {
        PutString(static_cast<const char*>(buffer), count, 0xFFFFFF, 0);
    }
}