#include "sys/syscall.h"
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <readline.h>
#include <sys/wait.h>

char PWD[256];
char prompt[263] = "root:";

void set_prompt(void)
{
    int i;
    for (i = 5; i < 256; ++i)
    {
        prompt[i] = PWD[i-5];
        if (!PWD[i-5]) break;
    }
    prompt[i++] = '$';
    prompt[i++] = ' ';
    prompt[i]   = 0;
}

void cd(int argc, char **argv)
{
    if (argc > 2)
    {
        puts("cd: too many arguments");
        return;
    }
    if (argc == 1) return; // todo: cd to ~/

    chdir(argv[1]);
    getcwd(PWD, 256); // better to call getcwd since the kernel does a lot of modifications
    set_prompt();
}

int main()
{
    getcwd(PWD, 256);
    set_prompt();

    fputs("ViperOS Shell\n", stdout);
    char *buf = malloc(256);
    memcpy(buf, "/bin/", 5);
    while (1)
    {
        readline(buf+5, prompt);
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

        if (!strcmp(buf+5, "cd"))
        {
            cd(brks, argv);
            continue;
        }

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
