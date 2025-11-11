#include "readline.h"

#include "poll.h"
#include "string.h"
#include "unistd.h"

char *readline(char *buf, char *prompt)
{
    int sz = 0;
    if (prompt)
    {
        write(1, prompt, strlen(prompt));
        write(1, "\x1b""f", 3); // flush
    }

    char c[3] = {0, '\x1b', 'f'}; // flush
    int done = 0;
    while (1)
    {
        while (read(0, &c, 1) > 0)
        {
            if (c[0] == '\b')
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
                if (c[0] == '\n')
                {
                    done = 1;
                    break;
                }
                buf[sz++] = c[0];
            }
        }
        if (done) break;
        poll1(0);
    }
    buf[sz] = 0;
    return buf;
}
