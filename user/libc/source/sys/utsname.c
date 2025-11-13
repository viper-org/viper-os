#include <sys/utsname.h>
#include <sys/syscall.h>

int uname(struct utsname *buf)
{
    return syscall1(SYS_uname, (uint64_t)buf);
}
