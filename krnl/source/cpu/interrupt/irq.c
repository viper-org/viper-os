#include "cpu/cpu.h"
#include "cpu/int/lapic.h"

#include "sched/sched.h"

#define TIMER_VEC 0x20

void irq_handler(struct cpu_state *state)
{
    lapic_eoi();
    if (state->base_frame.vector == TIMER_VEC)
    {
        sched_tick();
    }
}
