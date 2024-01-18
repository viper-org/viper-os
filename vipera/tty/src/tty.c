#include <screen.h>
#include <syscall.h>

size_t x = 0;
size_t y = 0;

void putchar(char c, uint32_t fg)
{
    switch(c)
    {
        case '\n':
        {
            x = 0;
            ++y;
            flush();
            break;
        }
        default:
        {
            plot_char(c, x, y, fg, 0);
            if (++x >= get_horiz() / 8)
            {
                x = 0;
                ++y;
            }
            break;
        }
    }
    // TODO: scrolling
}

void puts(const char* str, uint32_t fg)
{
    while(*str)
        putchar(*str++, fg);
}

void _start(void)
{
    screen_init();
    puts("Hello from terminal emulator\n", 0x00ff00);

    int fds[2];
    pipe(fds);

    write(fds[1], "hi", 2);
    char buf[2];
    while(1)
    {
        size_t count = read(fds[0], buf, 1);
        for (size_t i = 0; i < count; ++i)
        {
            putchar(buf[i], 0xffffff);
        }
        flush();
    }
    
    while(1) asm("pause");
}