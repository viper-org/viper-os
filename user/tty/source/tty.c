#include "screen.h"

#include <unistd.h>
#include <poll.h>

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

void setup_pipes(int fds[2])
{
    pipe(fds);
    // move the read end to 13 so 0 can be used for stdin
    // move write end to 1 for stdout
    int newfds[2];
    newfds[0] = dup2(fds[0], 13);
    newfds[1] = dup2(fds[1], 1);
    close(fds[0]);
    close(fds[1]);
    fds[0] = newfds[0];
    fds[1] = newfds[1];
}

void mainloop(int fds[2])
{
    char c;
    while(1)
    {
        poll1(fds[0]);
        while(read(fds[0], &c, 1))
        putchar(c, 0xffffff);
    }
}

void _start(void)
{
    screen_init();

    int fds[2];
    setup_pipes(fds);
    puts("test", 0xFFFFFFFF);
    flush();
    mainloop(fds);
}
