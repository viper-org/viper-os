#include "lexer.h"

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

    if (chdir(argv[1]) < 0)
    {
        fprintf(stderr, "cd: could not open '%s'\n", argv[1]);
        return;
    }
    getcwd(PWD, 256); // better to call getcwd since the kernel does a lot of modifications
    set_prompt();
}

int main(void)
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

        struct lexer l;
        l.l = buf;
        lex(&l);
        int argc = 0;
        int stdoutfd = SPAWN_INHERITFD;
        for (int i = 0; i < l.ntok; ++i)
        {
            if (l.toks[i].type == TOK_STR) ++argc;
            else if (l.toks[i].type == TOK_PIPER || l.toks[i].type == TOK_PIPER2)
            {
                int flags = O_WRONLY | O_CREAT;
                if (l.toks[i].type == TOK_PIPER2) flags |= O_APPEND;
                ++i;
                if (l.toks[i].type != TOK_STR)
                {
                    fprintf(stderr, "sh: expected a file before `newline'\n");
                    lexer_cleanup(&l);
                    continue;
                }
                stdoutfd = open(l.toks[i++].text, flags);
            }
        }
        char **argv = malloc(sizeof (char *) * (argc + 1));
        for (int i = 0; i < l.ntok; ++i)
        {
            if (l.toks[i].type == TOK_STR)
                argv[i] = l.toks[i].text;
        }
        argv[l.ntok] = NULL;

        int fd = open(argv[0], O_RDWR);
        if (fd >= 0)
        {
            close(fd);
            struct spawn spawnbuf = { -1, stdoutfd, -1 };
            int pid = spawn(l.toks[0].text, argc, argv, &spawnbuf);
            int _;
            waitpid(pid, &_, 0);
        }
        else
        {
            printf("%s: command not found\n", argv[0] + 5);
        }
        free(argv);
        lexer_cleanup(&l);
        if (stdoutfd >= 0) close(stdoutfd);
    }
    _exit(0);
}
