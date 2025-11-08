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
            write(1, &c, 1);
            if (c == '\b')
            {
                linebuf[--p] = '\0';
            }
            else
            {
                linebuf[p++] = c;
                if (c == '\n')
                {
                    done = 1;
                    break;
                }
            }
        }
        if (done) break;
    }
    write(1, linebuf, p);
    _exit(0);
}
