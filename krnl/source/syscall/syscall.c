#include "syscall/syscalls.h"

#include "driver/dbg.h"

#include <stdint.h>


struct syscall_frame
{
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rax;

    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbp;
    uint64_t rbx;

    uint64_t rflags;
    uint64_t rip;
    uint64_t rsp;
};

void syscall_dispatcher(struct syscall_frame *frame)
{
    switch (frame->rax)
    {
        case 2:
            frame->rax = sys_open((const char *)frame->rdi, frame->rsi);
            break;
        
        default:
            dbg_printf("syscall rax=%d", frame->rax);
            break;
    }
}
