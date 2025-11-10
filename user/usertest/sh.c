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
    char *buf = malloc(32);
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
        int fd = open(buf, O_RDWR);
        if (fd >= 0)
        {
            close(fd);
            int pid = spawn(buf);
            int _;
            waitpid(pid, &_, 0);
        }
        else
        {
            fputs(buf+5, stdout);
            fputs(": command not found\n", stdout);
        }
    }
    _exit(0);
}
