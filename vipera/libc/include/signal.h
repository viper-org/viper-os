#ifndef VIPEROS_VIPERA_LIBC_SIGNAL_H
#define VIPEROS_VIPERA_LIBC_SIGNAL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef VIPEROS_VIPERA_LIBCPP_CSIGNAL
namespace std {
#endif

#define SIGHUP 1
#define SIGINT 2
#define SIGQUIT 3
#define SIGILL 4
#define SIGTRAP 5
#define SIGABRT 6
#define SIGIOT 6
#define SIGBUS 7
#define SIGFPE 8
#define SIGKILL 9
#define SIGUSR1 10
#define SIGSEGV 11
#define SIGUSR2 12
#define SIGPIPE 13
#define SIGALRM 14
#define SIGTERM 15
#define SIGSTKFLT 16
#define SIGCHLD 17
#define SIGCONT 18
#define SIGSTOP 19
#define SIGTSTP 20
#define SIGTTIN 21
#define SIGTTOU 22
#define SIGURG 23
#define SIGXCPU 24
#define SIGXFSZ 25
#define SIGVTALRM 26
#define SIGPROF 27
#define SIGWINCH 28
#define SIGIO 29
#define SIGPWR 30
#define SIGSYS 31
#define SIGUNUSED 31

void (*signal(int sig, void (*handler) (int))) (int);
int raise(int sig);

#ifdef VIPEROS_VIPERA_LIBCPP_CSIGNAL
}
#endif

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_SIGNAL_H