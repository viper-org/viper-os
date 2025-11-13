#include <signal.h>

#include <sys/syscall.h>

int sigaction(int signum, void (*handler)(int))
{
    return syscall2(SYS_sigaction, signum, (uint64_t)handler);
}
