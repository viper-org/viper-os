#include <drivers/Terminal.h>
#include <drivers/Framebuffer.h>

#include <fs/Module.h>
#include <fs/DevFs.h>

#include <stdarg.h>

namespace Framebuffer
{
    extern struct FB framebuffer;
}

int IToA(int64_t value, char *sp, int radix)
{
    char tmp[32];
    char *tp = tmp;
    int64_t i;
    int64_t v;

    int64_t sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (int64_t)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'A' - 10;
    }

    int64_t len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
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

    void VPrintf(const char* format, va_list arg)
    {
        int i = 0;
        while(format[i])
        {
            if (format[i] == '%')
            {
                if (format[++i] == '%')
                    PutChar(format[i++], 0xFFFFFF, 0);
                else if (format[i] == 'd' || format[i] == 'i')
                {
                    char buf[32];
                    int64_t n = va_arg(arg, int64_t);
                    int count = IToA(n, buf, 10);
                    PutString(buf, count, 0xFFFFFF, 0);
                    i++;
                }
                else if(format[i] == 'x')
                {
                    char buf[32] = "0x";
                    int64_t n = va_arg(arg, int64_t);
                    int count = IToA(n, buf + 2, 16);
                    PutString(buf, count + 2, 0xFFFFFF, 0);
                    i++;
                }
                else if (format[i] == 'c')
                {
                    int ch_int = va_arg(arg, int);
                    char ch = (char)ch_int;
                    PutChar(ch, 0xFFFFFF, 0);
                    i++;
                }
                else if (format[i] == 's')
                {
                    char* str = va_arg(arg, char*);
                    Print(str, 0xFFFFFF, 0);
                    i++;
                }
            }
            else
                PutChar(format[i++], 0xFFFFFF, 0);
        }
    }

    void Printf(const char* format, ...)
    {
        va_list arg;
        va_start(arg, format);
        VPrintf(format, arg);
        va_end(arg);
    }


    void DriverWrite(const void* buffer, size_t count)
    {
        PutString(static_cast<const char*>(buffer), count, 0xFFFFFF, 0);
    }
}