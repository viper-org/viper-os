#include "screen.h"
#include "escape.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

size_t x = 0;
size_t y = 0;

void scroll(void);

int doing_escape = 0;
char escapebuf[16];
int escapebufp = 0;
enum tty_mode mode = TTY_COOKED;
char *linebuf;
int linebufp;

char *charbuf;

void end_escape(void)
{
    doing_escape = 0;
    memset(escapebuf, 0, escapebufp);
    escapebufp = 0;
}

void putchar(char c, uint32_t fg)
{
    if (doing_escape)
    {
        escapebuf[escapebufp++] = c;

        if (escapebuf[0] == 'M' && escapebufp >= 2)
        {
            if (escapebuf[1] == 'r') mode = TTY_RAW;
            else mode = TTY_COOKED;
            end_escape();
            return;
        }
        else if (escapebuf[0] == '[')
        {
            if (!memcmp(escapebuf, "[2J", 3))
            {
                clear_screen();
                end_escape();
                return;
            }
            else if (escapebuf[1] == 'H')
            {
                x = y = 0;
                end_escape();
                return;
            }
            else
            {
                int i = 1;
                while (isdigit(escapebuf[i]) && i < escapebufp) ++i;
                int n = atoi(escapebuf+1);
                switch (escapebuf[i])
                {
                    case 'A':
                    {
                        y -= n;
                        break;
                    }
                    case 'B':
                    {
                        y += n;
                        break;
                    }
                    case 'C':
                    {
                        x += n;
                        break;
                    }
                    case 'D':
                    {
                        x -= n;
                        break;
                    }
                    default: return;
                }
                end_escape();
                return;
            }
        }
        return;
    }

    if (y >= get_vert() / 8)
    {
        scroll();
    }

    switch(c)
    {
        case '\0': break;
        case '\n':
        {
            x = 0;
            ++y;
            flush();
            break;
        }
        case '\b':
        {
            --x;
            if (x < 0) --y, x = get_horiz() / 8;
            plot_char(' ', x, y, fg, 0);
            charbuf[x + y * get_horiz() / 8] = 0;
            break;
        }
        case '\x1b':
        {
            doing_escape = 1;
            break;
        }
        default:
        {
            plot_char(c, x, y, fg, 0);
            charbuf[x + y * get_horiz() / 8] = c;
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

void putstrn(const char *str, int n, uint32_t fg)
{
    while (n--)
        putchar(*str++, fg);
}

void putstr(const char* str, uint32_t fg)
{
    while(*str)
        putchar(*str++, fg);
}

void scroll(void)
{
    clear_screen();


    for (int x = 0; x < get_horiz() / 8; ++x)
    {
        for (int y = 0; y < get_vert() / 8 - 1; ++y)
        {
            charbuf[x + y * get_horiz() / 8] = charbuf[x + (y + 1) * get_horiz() / 8];
            plot_char(charbuf[x + y * get_horiz() / 8], x, y, 0xfffffff, 0);
        }
    }
    int last_y = get_vert() / 8 - 1;
    memset(charbuf + last_y * get_horiz() / 8, 0, get_horiz() / 8);

    --y;
}

void setup_pipes(int stdoutfds[2], int stdinfds[2])
{
    pipe(stdoutfds);
    // move the read end to 13 so 0 can be used for stdin
    // move write end to 1 for stdout
    int newfds[2];
    newfds[0] = dup2(stdoutfds[0], 13);
    newfds[1] = dup2(stdoutfds[1], 1);
    if (stdoutfds[0] != newfds[0]) close(stdoutfds[0]);
    if (stdoutfds[1] != 1) close(stdoutfds[1]);
    stdoutfds[0] = newfds[0];
    stdoutfds[1] = newfds[1];


    pipe(stdinfds);
    newfds[1] = dup2(stdinfds[1], 12);
    newfds[0] = dup2(stdinfds[0], 0);
    if (stdinfds[0] != 0) close(stdinfds[0]);
    if (stdinfds[1] != newfds[1]) close(stdinfds[1]);
    stdinfds[0] = newfds[0];
    stdinfds[1] = newfds[1];
}

struct keyboard_event
{
    uint8_t mode;
    uint8_t scancode;
};

static uint32_t scancode_map[128] = {
    0, -ESC_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', -ESC_CTRL,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    -ESC_SHIFT, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', -ESC_SHIFT,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -ESC_UP, 0, '-', -ESC_LEFT, 0, -ESC_RIGHT, '+', 0, -ESC_DOWN
};

static uint32_t scancode_map_shift[128] = {
    0, -ESC_ESC, '!', '"', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', -ESC_CTRL,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '@', '|',
    -ESC_SHIFT, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', -ESC_SHIFT,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -ESC_UP, 0, '-', -ESC_LEFT, 0, -ESC_RIGHT, '+', 0, -ESC_DOWN
};

int shift_down = 0;

void mainloop(int stdoutfds[2], int stdinfds[2])
{
    struct keyboard_event kbuf[32];
    char cbuf[32];
    int kbd = open("/dev/kb", O_RDONLY);
    int pollfds[2] = {kbd,stdoutfds[0]};
    while(1)
    {
        int readfd = poll(pollfds, 2);
        if (readfd == kbd)
        {
            int sz = 0;
            while((sz = read(readfd, kbuf, 32 * sizeof (struct keyboard_event))))
            {
                for (int i = 0; i < sz / sizeof (struct keyboard_event); ++i)
                {
                    int ch = scancode_map[kbuf[i].scancode];
                    switch (mode)
                    {
                        case TTY_RAW:
                        {
                            if (ch < 0)
                            {
                                char c = -ch;
                                char mode = kbuf[i].mode & 0x80 ? 'U' : 'P';
                                char buf[] = {'\x1b', mode, c};
                                write(stdinfds[1], buf, 3);
                            }
                            else
                            {
                                if (!(kbuf[i].mode & 0x80))
                                {
                                    char c = ch;
                                    write(stdinfds[1], &c, 1);
                                }
                            }
                            break;
                        }
                        case TTY_COOKED:
                        {
                            if (shift_down) ch = toupper(scancode_map_shift[kbuf[i].scancode]);
                            if (ch == -ESC_SHIFT)
                            {
                                if (kbuf[i].mode & 0x80) shift_down = 0;
                                else shift_down = 1;
                            }
                            if (!(kbuf[i].mode & 0x80))
                            {
                                char c = ch;
                                if (ch > 0)
                                {
                                    linebuf[linebufp++] = c;
                                    putchar(c, 0xffffffff);
                                    if (c == '\n')
                                    {
                                        linebuf[linebufp++] = 0;
                                        write(stdinfds[1], linebuf, linebufp);
                                        linebufp = 0;
                                    }
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (readfd == stdoutfds[0])
        {
            int sz = 0;
            while((sz = read(readfd, cbuf, 32)))
            {
                putstrn(cbuf, sz, 0xfffffff);
            }
        }
    }
}

char *argv[] = { "/bin/sh", 0 };

int main()
{
    screen_init();

    linebuf = malloc(512);
    linebufp = 0;
    charbuf = malloc((get_vert() * get_horiz()) / 64);

    int stdoutfds[2];
    int stdinfds[2];
    setup_pipes(stdoutfds, stdinfds);
    struct spawn spawnbuf = { -1, -1, -1 };
    spawn("/bin/sh", 1, argv, &spawnbuf);
    mainloop(stdoutfds, stdinfds);
}
