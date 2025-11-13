#include "cpu/cpu.h"
#include "cpu/int/lapic.h"

#include "driver/keyboard.h"

#include "sched/sched.h"

#include "signal/signal.h"

#define TIMER_VEC 0x20
#define KB_VEC 0x21

void irq_handler(struct cpu_state *state)
{
    lapic_eoi();
    if (state->base_frame.vector == TIMER_VEC)
    {
        sched_tick();
    }
    else if (state->base_frame.vector == KB_VEC)
    {
        kb_handler();
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
