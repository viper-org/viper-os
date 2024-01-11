#ifndef VIPEROS_ATHERIS_X64_ATHERIS_SCHED_USER_H
#define VIPEROS_ATHERIS_X64_ATHERIS_SCHED_USER_H 1

namespace echis::sched
{
    class Thread;
}

namespace atheris
{
    namespace sched
    {
        void UserInit(echis::sched::Thread* thread);
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_SCHED_USER_H