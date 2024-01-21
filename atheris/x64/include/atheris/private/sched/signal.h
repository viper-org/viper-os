#ifndef VIPEROS_ATHERIS_X64_ATHERIS_SCHED_IDLE_H
#define VIPEROS_ATHERIS_X64_ATHERIS_SCHED_IDLE_H 1

namespace atheris
{
    namespace sched
    {
        void SignalHandler(void(*handler)(int));
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_SCHED_IDLE_H
