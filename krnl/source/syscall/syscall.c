#include "syscall/syscalls.h"

#include "ldr/elf.h"

#include "mm/vm.h"

#include "signal/signal.h"

#include "driver/dbg.h"

#include "cpu/cpu.h"

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

        case 11:
            frame->rax = sys_munmap((void *)frame->rdi, frame->rsi);
            break;

        case 12:
            frame->rax = sys_waitpid(frame->rdi, (int *)frame->rsi, frame->rdx);
            break;
        
        case 13:
            frame->rax = sys_getpid();
            break;

        case 14:
            frame->rax = sys_getppid();
            break;

        case 16:
            frame->rax = sys_ioctl(frame->rdi, frame->rsi, (char *)frame->rdx);
            break;

        case 17:
            frame->rax = sys_poll((int *)frame->rdi, frame->rsi);
            break;

        case 18:
            frame->rax = sys_sigaction(frame->rdi, (void(*)(int))frame->rsi);
            break;

        case 22:
            frame->rax = sys_pipe((int *)frame->rdi);
            break;

        case 24:
            sys_yield();
            break;

        case 25:
            frame->rax = sys_spawn((const char *)frame->rdi, frame->rsi, (char **)frame->rdx, (struct spawn *)frame->r10);
            break;

        case 33:
            frame->rax = sys_dup(frame->rdi);
            break;
        
        case 34:
            frame->rax = sys_dup2(frame->rdi, frame->rsi);
            break;

        case 59: // load a dynamic library
        {
            struct elf_exec e = load_elf((void *)frame->rdi, vm_get_addrspace());
            *(struct elf_exec *)frame->rsi = e;
            frame->rax = 0;
            break;
        }

        case 60:
            sys_exit(frame->rdi);
            break; // shouldn't return but avoid warnings by breaking

        case 67: // debug log
            dbg_print((const char *)frame->rdi);
            frame->rax = 0;
            break;

        case 68:
            dbg_printf("%p\n", frame->rdi);
            break;

        case 79:
            frame->rax = (uint64_t)sys_getcwd((char *)frame->rdi, frame->rsi);
            break;

        case 80:
            frame->rax = sys_chdir((char *)frame->rdi);
            break;

        case 83:
            frame->rax = sys_mkdir((const char *)frame->rdi);
            break;
        
        default:
            dbg_printf("syscall rax=%d", frame->rax);
            break;
    }

    struct pending_signal *sig;
    if ((sig = check_incoming()))
    {
        struct saved_user_ctx ctx = {
            frame->rip, get_core()->ustack, frame->rbp, frame->rflags,
            frame->rax, frame->rbx, 0,          frame->rdi,
            frame->rsi, frame->rdi, frame->r8, frame->r9,
            frame->r10, 0,          frame->r12, frame->r13,
            frame->r14, frame->r15
        };
        void (*handler)(int) = sig_gethandler(sig);
        int oldtype = sig->type;
        sig->type = SIGNONE;
        signal_handler(&ctx, handler, signal_return_to(oldtype));
    }
}
