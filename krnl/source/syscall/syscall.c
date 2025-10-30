#include "syscall/syscalls.h"

#include "sched/sched.h"

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
        case 0:
            frame->rax = sys_read(frame->rdi, (void *)frame->rsi, frame->rdx);
            break;
            
        case 1:
            frame->rax = sys_write(frame->rdi, (const void *)frame->rsi, frame->rdx);
            break;

        case 2:
            frame->rax = sys_open((const char *)frame->rdi, frame->rsi);
            break;

        case 3:
            frame->rax = sys_close(frame->rdi);
            break;

        case 4:
            frame->rax = sys_stat((const char *)frame->rdi, (struct stat *)frame->rsi);
            break;

        case 5:
            frame->rax = sys_fstat(frame->rdi, (struct stat *)frame->rsi);
            break;

        case 7:
            frame->rax = sys_poll1(frame->rdi);
            break;

        case 8:
            frame->rax = sys_lseek(frame->rdi, frame->rsi, frame->rdx);
            break;

        case 9:
            frame->rax = (uint64_t)sys_mmap((void *)frame->rdi, frame->rsi, frame->rdx, frame->r10, frame->r8, frame->r9);
            break;

        case 12:
            frame->rax = sys_waitpid(frame->rdi, (int *)frame->rsi, frame->rdx);
            break;
        
        case 13:
            frame->rax = sys_getpid();
            break;

        case 16:
            frame->rax = sys_ioctl(frame->rdi, frame->rsi, (char *)frame->rdx);
            break;

        case 22:
            frame->rax = sys_pipe((int *)frame->rdi);
            break;

        case 24:
            sys_yield();
            break;

        case 25:
            frame->rax = sys_spawn((const char *)frame->rdi);
            break;

        case 33:
            frame->rax = sys_dup(frame->rdi);
            break;
        
        case 34:
            frame->rax = sys_dup2(frame->rdi, frame->rsi);
            break;

        case 60:
            sys_exit(frame->rdi);
            break; // shouldn't return but avoid warnings by breaking

        case 67: // debug log
            dbg_print((const char *)frame->rdi);
            frame->rax = 0;
            break;
        
        default:
            dbg_printf("syscall rax=%d", frame->rax);
            break;
    }
}
