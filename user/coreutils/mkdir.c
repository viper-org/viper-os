#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fputs("mkdir: missing operand\n", stderr);
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        mkdir(argv[i]);
    }
}
