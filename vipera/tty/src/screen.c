#include <screen.h>
#include <string.h>
#include <syscall.h>
#include <font.h>


void* back_buffer = NULL;
int   framebuffer_fd = -1;

void screen_init()
{
    framebuffer_fd = open("dev:fb", WRITE);
    framebuffer_fd = movfd(framebuffer_fd, 15); // 0, 1, 2 are reserved

    back_buffer = mmap(get_vert() * get_pitch());
    
    clear_screen();
    flush();
    font_init();
}


size_t get_horiz()
{
    static size_t horiz = 0;
    if (!horiz)
    {
        ioctl(framebuffer_fd, 0, &horiz);
    }
    return horiz;
}

size_t get_pitch()
{
    static size_t pitch = 0;
    if (!pitch)
    {
        ioctl(framebuffer_fd, 2, &pitch);
    }
    return pitch;
}

size_t get_vert()
{
    static size_t vert = 0;
    if (!vert)
    {
        ioctl(framebuffer_fd, 1, &vert);
    }
    return vert;
}


void clear_screen()
{
    memset(back_buffer, 0, get_vert() * get_pitch());
}

void flush()
{
    write(framebuffer_fd, back_buffer, get_vert() * get_pitch());
}


void draw_bitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
{
    uint8_t* address = back_buffer;
    uint32_t index = x * 4 + y * get_pitch();

    uint8_t* bitmap_data = bitmap;

    for (uint32_t i = 0; i < 8; ++i)
    {
        uint8_t row = bitmap_data[i];

        for (uint32_t j = 0; j < 8; j++)
        {
            uint32_t color = (row >> (8 - j - 1) & 0x1) ? fg : bg;

            address[index]     =  color        & 0xff;
            address[index + 1] = (color >> 8)  & 0xff;
            address[index + 2] = (color >> 16) & 0xff;

            index += 4;
        }

        index += get_pitch();
        index -= 32;
    }
}

void plot_char(unsigned char c, int x, int y, uint32_t fg, uint32_t bg)
{
    char font[8];
    read_font(c, font);
    draw_bitmap(font, x * 8, y * 8, fg, bg);
}