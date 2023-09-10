#ifndef VIPER_OS_SCHED_PROCESS_H
#define VIPER_OS_SCHED_PROCESS_H 1

#include <cpu/Context.h>

namespace sched
{
    class Process
    {
    public:
        enum class Privilege
        {
            User,
            Kernel
        };

        Process();

        Process(uint64_t rip);

        [[noreturn]] void launch();

        cpu::Context* getContext();

    private:
        cpu::Context mContext;
        Privilege mPrivilege;
    };
}

#endif