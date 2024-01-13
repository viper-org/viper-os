#include <atheris/private/driver/timer.h>

#include <driver/hpet.h>

#include <cpu/interrupt/apic.h>
#include <cpu/core.h>

namespace atheris
{
    namespace timer
    {
        void Init()
        {
            x64::cpu::apic::InitTimer();
        }

        void Subscribe(TimerHandler newHandler)
        {
            cpu::core::CoreLocal::Get()->timerHandler = newHandler;
        }

        void TimerInterruptHandler()
        {
            if (auto handler = cpu::core::CoreLocal::Get()->timerHandler)
            {
                handler();
            }
        }
    }
}