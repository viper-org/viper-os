#include <stdio.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        puts("no directory specified");
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if (!dir)
    {
        printf("cannot access '%s'", argv[1]);
        return 1; // todo: use perror()
    }

    struct dirent d;
    while (readdir(dir, &d) >= 0)
    {
        printf(" - %s\n", d.d_name);
    }
}
