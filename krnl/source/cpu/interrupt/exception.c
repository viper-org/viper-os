#include "cpu/cpu.h"

#include "driver/dbg.h"

void exception_handler(struct cpu_state *state)
{
    dbg_printf("\nreceived exception: %d\n", state->base_frame.vector);
    for (;;) __asm__("cli; pause");
}
