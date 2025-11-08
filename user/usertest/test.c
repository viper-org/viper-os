#include <poll.h>

#include <unistd.h>

void _start(void)
{
    write(1, "Hello, world!\n", 15);
    char linebuf[32];
    int p = 0;
    char c;
    int done = 0;
    while (1)
    {
        poll1(0);
        while (read(0, &c, 1))
        {
            linebuf[p++] = c;
            if (c == '\n')
            {
                done = 1;
                break;
            }
        }
        if (done) break;
    }
    write(1, linebuf, p);
    while(1) asm("pause");
}
