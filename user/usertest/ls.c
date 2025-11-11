#include "unistd.h"
#include <stdio.h>
#include <dirent.h>

void do_ls(char *path)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        printf("cannot access '%s'\n", path); // todo: use perror()
        _exit(1); // todo: exit()
    }

    struct dirent d;
    while (readdir(dir, &d) >= 0)
    {
        printf(" - %s\n", d.d_name);
    }
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        char buf[256];
        getcwd(buf, 256);
        do_ls(buf);
    }
    else
    {
        do_ls(argv[1]);
    }
}
