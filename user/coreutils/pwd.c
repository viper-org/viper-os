#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char buf[256];
    if (!getcwd(buf, 256))
    {
        fputs("pwd: failed to read working directory\n", stderr);
        return 1;
    }
    puts(buf);
}
