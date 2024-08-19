#include <cpu/interrupt/apic.h>
#include <cpu/cpu.h>

#include <atheris/driver/timer.h>

namespace atheris
{
    namespace amd64
    {
        extern "C" void CommonIRQHandler(cpu::State* context)
        {
            cpu::apic::SendEOI();
            if (context->BaseFrame.vector == atheris::timer::TimerInterruptVector)
            {
                atheris::timer::TimerInterruptHandler();
            }
        }
    }
}