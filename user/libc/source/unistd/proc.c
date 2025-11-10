#include <unistd.h>
#include <sys/syscall.h>

pid_t spawn(const char *path, int argc, char **argv)
{
    return syscall3(SYS_spawn, (uint64_t)path, argc, (uint64_t)argv);
}

pid_t getpid(void)
{
    return syscall0(SYS_getpid);
}

pid_t getppid(void)
{
    return syscall0(SYS_getppid);
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

