#include "font.h"

#include <unistd.h>

int fontFd = -1;

void font_init(void)
{
    if (fontFd == -1)
    {
        fontFd = open("/etc/font.bmp", O_RDONLY);
        int newfd = dup2(fontFd, 14); // 0, 1, 2 are reserved
        close(fontFd);
        fontFd = newfd;
    }
}

void read_font(unsigned char c, char data[8])
{
    lseek(fontFd, c * 8, SEEK_SET);
    read(fontFd, data, 8);
}
