#include "readline.h"

#include "poll.h"
#include "string.h"
#include "unistd.h"

char *readline(char *buf, char *prompt)
{
    int sz = 0;
    write(1, "\x1b""Mr", 3);
    if (prompt)
    {
        write(1, prompt, strlen(prompt));
    }

    char c = 0;
    int done = 0;
    while (1)
    {
        while (read(0, &c, 1) > 0)
        {
            if (c == '\b')
            {
                if (sz)
                {
                    write(1, &c, 3);
                    buf[--sz] = '\0';
                }
            }
            else
            {
                write(1, &c, 3);
                if (c == '\n')
                {
                    done = 1;
                    break;
                }
                buf[sz++] = c;
            }
        }
        if (done) break;
        poll1(0);
    }
    buf[sz] = 0;
    write(1, "\x1b""Mc", 3);
    return buf;
}
