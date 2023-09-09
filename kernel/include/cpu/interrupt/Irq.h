#ifndef VIPER_OS_CPU_INTERRUPT_IRQ_H
#define VIPER_OS_CPU_INTERRUPT_IRQ_H 1

#include <cpu/interrupt/Interrupt.h>

#include <container/function.h>

namespace irq
{
    using Handler = void(*)(InterruptFrame*);

    void subscribe(int irq, Handler callback);
}

#endif