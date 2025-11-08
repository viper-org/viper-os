#include <poll.h>

#include <unistd.h>
#include <readline.h>
#include <sys/wait.h>

void _start(void)
{
    write(1, "ViperOS Shell\n", 15);
    while (1)
    {
        char buf[32] = "/bin/";
        readline(buf+5, "> ");
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
            write(1, "command not found\n", 19);
        }
    }
    _exit(0);
}
