#include "screen.h"
#include "sys/syscall.h"

#include <unistd.h>
#include <poll.h>

size_t x = 0;
size_t y = 0;

int doing_escape = 0;

void putchar(char c, uint32_t fg)
{
    if (doing_escape)
    {
        if (c == 'f') flush();
        doing_escape = 0;
        return;
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
    0, -1, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', -2,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    -3, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', -4,
    '*', -5, ' '
};

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
                syscall1(67, (uint64_t)"TEST\n\n\n");
                syscall1(68, sz);
                for (int i = 0; i < sz / sizeof (struct keyboard_event); ++i)
                {
                    uint32_t ch = scancode_map[kbuf[i].scancode];
                    if (kbuf[i].mode & 0x80)
                    {
                        char c = ch;
                        write(stdinfds[1], &c, 1);
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

    int stdoutfds[2];
    int stdinfds[2];
    setup_pipes(stdoutfds, stdinfds);
    spawn("/bin/sh", 1, argv);
    mainloop(stdoutfds, stdinfds);
}
