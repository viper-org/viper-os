#ifndef VIPEROS_LIBC_SIGNAL
#define VIPEROS_LIBC_SIGNAL 1

typedef int pid_t;

enum signal_type
{
    SIGNONE=-1,
    SIGHUP=1,
    SIGINT=2,
    SIGQUIT=3,
    SIGILL=4,
    SIGTRAP=5,
    SIGABRT=6,
    SIGIOT=6,
    SIGBUS=7,
    SIGFPE=8,
    SIGKILL=9,
    SIGUSR1=10,
    SIGSEGV=11,
    SIGUSR2=12,
    SIGPIPE=13,
    SIGALRM=14,
    SIGTERM=15,
    SIGSTKFLT=16,
    SIGCHLD=17,
    SIGCONT=18,
    SIGSTOP=19,
    SIGTSTP=20,
    SIGTTIN=21,
    SIGTTOU=22,
    SIGURG=23,
    SIGXCPU=24,
    SIGXFSZ=25,
    SIGVTALRM=26,
    SIGPROF=27,
    SIGWINCH=28,
    SIGIO=29,
    SIGPWR=30,
    SIGSYS=31,
    SIGUNUSED=31
};

int sigaction(int signum, void (*handler)(int));

int raise(int sig);
int kill(pid_t pid, int sig);

#endif // VIPEROS_LIBC_SIGNAL
