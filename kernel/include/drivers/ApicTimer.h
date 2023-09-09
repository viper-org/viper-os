#ifndef VIPER_OS_DRIVERS_TIMER_APIC_H
#define VIPER_OS_DRIVERS_TIMER_APIC_H 1

#include <container/function.h>

namespace timer
{
    using Handler = vpr::function<void()>;

    void Init();

    void subscribe(Handler handler);
}

#endif // VIPER_OS_DRIVERS_TIMER_APIC_H