#include "readline.h"

#include "poll.h"
#include "string.h"
#include "sys/syscall.h"
#include "unistd.h"

enum
{
    STATE_SHIFT = 1 << 0,
};

enum key_escape
{
    ESC_CTRL = 1,
    ESC_SHIFT,
    ESC_UP,
    ESC_LEFT,
    ESC_DOWN,
    ESC_RIGHT,
    ESC_ESC
};

static const char shifted[127] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    ' ', '!', '"', '#', '$', '%', '&', '"', '(', ')', '*', '+', '<', '_', '>', '?',
    ')', '!', '@', '#', '$', '%', '^', '&', '*', '(',
    0, ':', '<', '+', '>', '?', 0,
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '{', '|', '}', '~', 0,
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    '{', '|', '}', '~'
};

char shift_char(char c)
{
    return shifted[c];
}

char *readline(char *buf, char *prompt)
{
    int sz = 0;
    int state = 0;
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
                    write(1, &c, 1);
                    buf[--sz] = '\0';
                }
            }
            else if (c == '\x1b')
            {
                read(0, &c, 1);
                char mode = c;
                read(0, &c, 1);
                syscall1(68, mode);
                if (c == ESC_SHIFT)
                {
                    if (mode == 'U') state &= ~STATE_SHIFT;
                    if (mode == 'P') state |=  STATE_SHIFT;
                }
            }
            else
            {
                if (state & STATE_SHIFT) c = shift_char(c);
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
        poll1(0);
    }
    buf[sz] = 0;
    write(1, "\x1b""Mc", 3);
    return buf;
}
