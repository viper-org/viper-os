#include <drivers/ApicTimer.h>
#include <drivers/Hpet.h>
#include <drivers/Apic.h>

#include <cpu/interrupt/Irq.h>

#include <container/lazy.h>
#include <container/list.h>

namespace timer
{
    vpr::lazy<vpr::list<Handler> > handlers;

    void TimerHandler(InterruptFrame*)
    {
        for (const auto& handler : *handlers)
        {
            handler();
        }
        APIC::SendEOI();
    }

    constexpr int DIV_CONFIG = 0x3E0;
    constexpr int INIT_CNT   = 0x380;
    constexpr int CURR_CNT   = 0x390;
    constexpr int LVT        = 0x320;

    void Init()
    {
        irq::subscribe(0, TimerHandler);
        
        APIC::WriteRegister(DIV_CONFIG, 0x3);

        APIC::WriteRegister(INIT_CNT, 0xFFFFFFFF); // -1
        HPET::Sleep(10e12); // 10ms

        APIC::WriteRegister(LVT, 1 << 16); // Mask

        uint32_t ticks = 0xFFFFFFFF - APIC::ReadRegister(CURR_CNT); // 10ms ticks

        APIC::WriteRegister(LVT, 32 | 0x20000); // IRQ 0, periodic
        APIC::WriteRegister(DIV_CONFIG, 0x3);
        APIC::WriteRegister(INIT_CNT, ticks);
    }

    void subscribe(Handler handler)
    {
        handlers->push_front(handler);
    }
}