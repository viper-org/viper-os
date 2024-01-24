#include <sys/unistd.h>
#include <sys/syscall.h>

long sysconf(int name)
{
    return syscall1(SYS_sysconf, name);
}