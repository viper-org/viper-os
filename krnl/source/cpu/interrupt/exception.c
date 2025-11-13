#include "cpu/cpu.h"

#include "driver/dbg.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "signal/signal.h"

#define INT_PF 14

void exception_handler(struct cpu_state *state)
{
    switch (state->base_frame.vector)
    {
        case INT_PF:
            break;

        default:
        {
            dbg_printf("\nreceived exception: %d\n", state->base_frame.vector);
            for (;;) __asm__("cli; pause");
        }
    }

    if (state->base_frame.vector == INT_PF)
    {
        if ((state->base_frame.cs & 0x3) == 0x3) // userspace
        {
            if (state->control_regs.cr2 == 0x519) // unreturnable signal
            {
                thread_kill(sched_curr(), 139);
            }
            else if (state->control_regs.cr2 == 0x516) // returnable signal
            {
                signal_return(state->base_frame.rsp);
            }
            deliver_to_current(SIGSEGV);
        }
        else
        {
            dbg_printf("\nreceived exception: %d\n", state->base_frame.vector);
            for (;;) __asm__("cli; pause");
        }
    }

    struct pending_signal *sig;
    if ((sig = check_incoming()))
    {
        struct saved_user_ctx ctx = {
            state->base_frame.rip, state->base_frame.rsp, state->base_frame.rbp, state->base_frame.rflags,
            state->general_regs.rax, state->general_regs.rbx, state->general_regs.rcx,
            state->general_regs.rdx, state->general_regs.rsi, state->general_regs.rdi,
            state->general_regs.r8, state->general_regs.r9, state->general_regs.r10,
            state->general_regs.r11, state->general_regs.r12, state->general_regs.r13,
            state->general_regs.r14, state->general_regs.r15
        };
        void (*handler)(int) = sig_gethandler(sig);
        int oldtype = sig->type;
        sig->type = SIGNONE;
        signal_handler(&ctx, handler, signal_return_to(oldtype));
    }
}
