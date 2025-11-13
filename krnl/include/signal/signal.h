#ifndef VIPEROS_SIGNAL_SIGNAL
#define VIPEROS_SIGNAL_SIGNAL 1

#include <stdint.h>

struct saved_user_ctx
{
    uint64_t rip;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rflags;

    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};

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

struct pending_signal
{
    enum signal_type type;

    struct pending_signal *next;
};

int signal_return_to(enum signal_type type);

void signal_handler(struct saved_user_ctx *ctx, void(*handler)(int), int return_to);
void signal_return(uint64_t rsp);

void deliver_to_current(enum signal_type sig);
struct pending_signal *check_incoming(void);

void (*sig_gethandler(struct pending_signal *))(int);

#endif // VIPEROS_SIGNAL_SIGNAL
