#ifndef VIPEROS_ECHIS_SCHED_EVENT_EVENT_H
#define VIPEROS_ECHIS_SCHED_EVENT_EVENT_H 1

#include <intrusive_list.h>

namespace echis
{
    namespace sched
    {
        class Thread;

        class Event
        {
        public:
            Event();
            virtual ~Event() {}

            virtual void onEmit();

            intrusive_list<Thread>& getWaitQueue();

        protected:
            intrusive_list<Thread> mWaitQueue;
        };
    }
}

#endif // VIPEROS_ECHIS_SCHED_EVENT_EVENT_H