#include <sched/event/event.h>

namespace echis
{
    namespace sched
    {
        Event::Event()
        {
        }

        void Event::onEmit()
        {
        }

        intrusive_list<Thread>& Event::getWaitQueue()
        {
            return mWaitQueue;
        }
    }
}