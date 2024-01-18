#include <font.h>
#include <syscall.h>

int fontFd = -1;

void read_font(unsigned char c, char data[8])
{
    if (fontFd == -1)
    {
        fontFd = open("tmp:font.bmp", READ);
    }
    seek(fontFd, c * 8);
    read(fontFd, data, 8);
}