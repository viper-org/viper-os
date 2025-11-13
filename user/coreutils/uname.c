#include <sys/utsname.h>
#include <stdio.h>

// todo: parse cmdline args to print requested data only

int main(void)
{
    struct utsname buf;
    uname(&buf); // maybe check return value here

    printf("%s %s %s %s %s\n", buf.sysname, buf.nodename, buf.release, buf.version, buf.machine);
}
