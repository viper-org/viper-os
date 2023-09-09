#ifndef VIPER_OS_DRIVERS_TERMINAL_H
#define VIPER_OS_DRIVERS_TERMINAL_H 1

#include <drivers/Framebuffer.h>

namespace Terminal
{
    void Init();

    void PutChar(char c, int x, int y, Framebuffer::Color fg, Framebuffer::Color bg);

    void PutChar(char c, Framebuffer::Color fg, Framebuffer::Color bg);
    void PutString(const char* data, unsigned int size, Framebuffer::Color fg, Framebuffer::Color bg);

    void Print(const char* data, Framebuffer::Color fg = 0xFFFFFF, Framebuffer::Color bg = 0);
}

#endif // VIPER_OS_DRIVERS_TERMINAL_H