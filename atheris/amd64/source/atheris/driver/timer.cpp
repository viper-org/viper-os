#include <atheris/driver/timer.h>

#include <driver/hpet.h>

#include <cpu/interrupt/apic.h>

namespace atheris
{
    namespace timer
    {
        void Init(int tickSizeNs)
        {
            amd64::cpu::apic::Init();
            amd64::hpet::Init();
            amd64::cpu::apic::InitTimer(tickSizeNs);
        }

        TimerHandler handler = nullptr; // TODO: Move this into core local structure

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