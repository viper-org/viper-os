#include "readline.h"

#include "poll.h"
#include "string.h"
#include "unistd.h"

char *readline(char *buf, char *prompt)
{
    int sz = 0;
    if (prompt)
        write(1, prompt, strlen(prompt));

    char c;
    int done = 0;
    while (1)
    {
        poll1(0);
        while (read(0, &c, 1))
        {
            if (c == '\b')
            {
                if (sz)
                {
                    write(1, &c, 1);
                    buf[--sz] = '\0';
                }
            }
            else
            {
                write(1, &c, 1);
                if (c == '\n')
                {
                    done = 1;
                    break;
                }
                buf[sz++] = c;
            }
        }
        if (done) break;
    }
    buf[sz] = 0;
    return buf;
}
