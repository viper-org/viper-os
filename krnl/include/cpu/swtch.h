#ifndef VIPEROS_CPU_SWTCH
#define VIPEROS_CPU_SWTCH 1

#include <stdint.h>

struct thread_context
{
    uint64_t rbp;
    uint64_t rbx;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
};

void swtch(struct thread_context **, struct thread_context *);

#endif // VIPEROS_CPU_SWTCH
