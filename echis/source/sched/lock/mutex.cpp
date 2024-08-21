#include <sched/lock/mutex.h>

#include <atheris/cpu/int.h>

namespace echis
{
    namespace sched
    {
        namespace lock
        {
            MutexUnlockEvent::MutexUnlockEvent(Mutex& mutex)
                    : mMutex(mutex)
            {
            }

            void MutexUnlockEvent::onEmit()
            {
                atheris::cpu::PushDisableInt();
                
                Thread* head = mWaitQueue.head();
                mWaitQueue.pop_front();
                sched::Unblock(head);

                atheris::cpu::PopDisableInt();
            }


            Mutex::Mutex()
                : mUnlockEvent(*this)
            {
            }

            void Mutex::lock()
            {
                bool expected = false;
                if (!mLock.compare_exchange(expected, true))
                {
                    atheris::cpu::PushDisableInt();
                    mUnlockEvent.getWaitQueue().push_back(sched::Current());
                    atheris::cpu::PopDisableInt();
                    sched::Block();
                }
            }

            void Mutex::unlock()
            {
                if (!mUnlockEvent.getWaitQueue().head()) // no threads waiting on the lock
                {
                    mLock.store(false);
                }
                else
                {
                    mUnlockEvent.onEmit();
                }
            }
        }
    }
}