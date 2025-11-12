#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fputs("cat: no input files\n", stderr);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "cat : %s: no such file or directory\n", argv[1]);
        return 1;
    }

    char buf[32];
    int read = 0;
    while ((read = fread(buf, 1, 32, f)) > 0)
    {
        fwrite(buf, 1, read, stdout);
    }
    puts("");
}
