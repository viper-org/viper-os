#include <unistd.h>
#include <sys/syscall.h>

pid_t spawn(const char *path)
{
    return syscall1(SYS_spawn, (uint64_t)path);
}

pid_t getpid(void)
{
    return syscall0(SYS_getpid);
}

void loadlib(const char *path, struct elf_exec *out)
{
    syscall2(SYS_loadlib, (uint64_t)path, (uint64_t)out);
}

void _exit(int status)
{
    syscall1(SYS_exit, status);
    __builtin_unreachable();
}

