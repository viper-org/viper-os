#include <atheris/private/driver/timer.h>

#include <driver/hpet.h>

#include <cpu/interrupt/apic.h>

namespace atheris
{
    namespace timer
    {
        static TimerHandler handler = nullptr;

        void Init()
        {
            x64::cpu::apic::InitTimer();
        }

        void Subscribe(TimerHandler newHandler)
        {
            handler = newHandler;
        }

        void TimerInterruptHandler()
        {
            if (handler)
            {
                handler();
            }
        }
    }
}