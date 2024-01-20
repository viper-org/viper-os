#include <font.h>
#include <syscall.h>

int fontFd = -1;

void font_init()
{
    if (fontFd == -1)
    {
        fontFd = open("tmp:font.bmp", READ);
    }
}

void read_font(unsigned char c, char data[8])
{
    seek(fontFd, c * 8);
    read(fontFd, data, 8);
}