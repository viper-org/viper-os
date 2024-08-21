#ifndef VIPEROS_ECHIS_SCHED_LOCK_MUTEX_H
#define VIPEROS_ECHIS_SCHED_LOCK_MUTEX_H 1

#include <sched/sched.h>
#include <sched/event/event.h>

#include <atomic>

namespace echis
{
    namespace sched
    {
        namespace lock
        {
            class Mutex;

            class MutexUnlockEvent : public Event
            {
            public:
                MutexUnlockEvent(Mutex& mutex);

                virtual void onEmit() override;

            private:
                Mutex& mMutex;
            };

            class Mutex
            {
            public:
                Mutex();

                void lock();
                void unlock();

            private:
                MutexUnlockEvent mUnlockEvent;
                std::atomic_bool mLock;
            };
        }
    }
}

#endif // VIPEROS_ECHIS_SCHED_LOCK_MUTEX_H