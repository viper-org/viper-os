#include "sys/syscall.h"
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <readline.h>
#include <sys/wait.h>

int main()
{
    fputs("ViperOS Shell\n", stdout);
    char *buf = malloc(256);
    memcpy(buf, "/bin/", 5);
    while (1)
    {
        readline(buf+5, "> ");
        if (!buf[5]) continue; // empty line
        if (!strcmp(buf+5, "exit"))
        {
            if (getppid() != 3)
            {
                _exit(0);
            }
            else
            {
                puts("cannot exit from root shell");
                continue;
            }
        }

        int len = strlen(buf);
        int brks = 1;
        for (int i = 0; i < len; ++i)
        {
            if (buf[i] == ' ')
            {
                buf[i] = 0;
                ++brks;
            }
        }
        char **argv = malloc((brks + 1) * sizeof(char *));
        char **argvp = argv;

        *(argvp++) = buf;
        for (int i = 0; i < len-1; ++i)
        {
            if (buf[i] == 0)
            {
                *(argvp++) = &buf[i+1];
                while (buf[i] == 0) ++i;
            }
        }
        *argvp = NULL;

        int fd = open(buf, O_RDWR);
        if (fd >= 0)
        {
            close(fd);
            int pid = spawn(buf, brks, argv);
            int _;
            waitpid(pid, &_, 0);
        }
        else
        {
            printf("%s: command not found\n", buf+5);
        }
    }
    _exit(0);
}
