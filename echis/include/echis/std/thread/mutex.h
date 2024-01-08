#ifndef VIPEROS_ECHIS_STD_THREAD_MUTEX_H
#define VIPEROS_ECHIS_STD_THREAD_MUTEX_H 1

#include <std/container/atomic.h>

namespace vpr
{
    class mutex
    {
    public:
        constexpr mutex()
        {
        }

        mutex(const mutex&) = delete;

        void lock();
        bool try_lock();
        void unlock();

    private:
        volatile atomic_bool locked;
    };


    template <class MutexType>
    class lock_guard
    {
    public:
        explicit lock_guard(MutexType& m)
            : mMutex(m)
        {
            m.lock();
        }

        lock_guard(const lock_guard&) = delete;

        ~lock_guard()
        {
            mMutex.unlock();
        }
    private:
        MutexType& mMutex;
    };
}

#endif // VIPEROS_ECHIS_STD_THREAD_MUTEX_H