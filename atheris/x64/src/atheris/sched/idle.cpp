#include <atheris/private/sched/idle.h>

namespace atheris
{
    namespace sched
    {
        extern "C" void idle();

        void StartIdleProcess()
        {
            idle();
        }
    }
}