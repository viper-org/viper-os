#ifndef VIPER_OS_CPU_INTERRUPT_EXCEPTION_H
#define VIPER_OS_CPU_INTERRUPT_EXCEPTION_H 1

#include <cpu/interrupt/Interrupt.h>

#include <container/function.h>

namespace exception
{
    using Handler = vpr::function<bool(InterruptFrame*)>;

    void Init();

    void subscribe(int vector, Handler callback);
}

#endif