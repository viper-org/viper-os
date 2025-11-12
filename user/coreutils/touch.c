#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fputs("touch: missing file operand\n", stderr);
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        close(open(argv[i], O_CREAT));
    }
}
