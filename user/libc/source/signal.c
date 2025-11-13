#include <signal.h>

#include <sys/syscall.h>

int sigaction(int signum, void (*handler)(int))
{
    return syscall2(SYS_sigaction, signum, (uint64_t)handler);
}

int raise(int sig)
{
    return syscall1(SYS_raise, sig);
}

int kill(pid_t pid, int sig)
{
    return syscall2(SYS_kill, pid, sig);
}
