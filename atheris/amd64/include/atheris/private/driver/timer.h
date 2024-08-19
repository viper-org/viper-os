#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_DRIVER_TIMER_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_DRIVER_TIMER_H 1

namespace atheris
{
    namespace timer
    {
        constexpr int TimerInterruptVector = 0x20;

        void Init(int tickSizeNs);

        using TimerHandler = void(*)();

        void Subscribe(TimerHandler handler);

        void TimerInterruptHandler();
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_DRIVER_TIMER_H