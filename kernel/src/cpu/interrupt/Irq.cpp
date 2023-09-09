#include <cpu/interrupt/Irq.h>
#include <cpu/interrupt/Interrupt.h>

#include <container/list.h>
#include <container/function.h>

#include <memory.h>

namespace irq
{
    Handler handlers[32];
    
    void subscribe(int vector, Handler callback)
    {
        handlers[vector] = callback;
    }

    extern "C" void CommonIRQHandler(InterruptFrame* frame)
    {
        if (handlers[frame->baseFrame.vector - 32])
        {
            handlers[frame->baseFrame.vector - 32](frame);
        }
    }
}