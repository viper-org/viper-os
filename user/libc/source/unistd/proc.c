#include <unistd.h>
#include <sys/syscall.h>

pid_t spawn(const char *path, int argc, char **argv, struct spawn *spawnbuf)
{
    return syscall4(SYS_spawn, (uint64_t)path, argc, (uint64_t)argv, (uint64_t)spawnbuf);
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

char *getcwd(char buf[], size_t size)
{
    return (char *)syscall2(SYS_getcwd, (uint64_t)buf, size);
}

int chdir(const char *path)
{
    return syscall1(SYS_chdir, (uint64_t)path);
}
