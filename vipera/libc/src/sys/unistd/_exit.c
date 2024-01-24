#include <sys/unistd.h>
#include <sys/syscall.h>

_Noreturn void _exit(int exit_code)
{
    syscall1(SYS_exit, exit_code);
    __builtin_unreachable();
}