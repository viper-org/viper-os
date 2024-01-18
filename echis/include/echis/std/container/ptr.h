#ifndef VIPEROS_ECHIS_STD_CONTAINER_PTR_H
#define VIPEROS_ECHIS_STD_CONTAINER_PTR_H 1

#include <std/container/atomic.h>

namespace vpr
{
    template <class T>
    class shared_ptr
    {
    public:
        shared_ptr()
            : mBase(nullptr)
            , mCount(nullptr)
        {
        }

        shared_ptr(decltype(nullptr))
            : shared_ptr()
        {
        }

        shared_ptr(T* data)
            : mBase(data)
            , mCount(new atomic_long(1))
        {
        }

        shared_ptr(const shared_ptr& other)
            : mBase(other.mBase)
            , mCount(other.mCount)
        {
            if (mCount)
            {
                *mCount += 1;
            }
        }

        ~shared_ptr()
        {
            if (mCount)
            {
                *mCount -= 1;
                if (*mCount == 0)
                {
                    delete mBase;
                    delete mCount;
                }
            }
        }

        shared_ptr& operator=(decltype(nullptr))
        {
            if (mCount)
            {
                *mCount -= 1;
                if (*mCount == 0)
                {
                    delete mBase;
                    delete mCount;
                    mCount = nullptr;
                }
            }
            mBase = nullptr;
            return *this;
        }

        shared_ptr& operator=(const shared_ptr& other)
        {
            if (mCount)
            {
                *mCount -= 1;
                if (*mCount == 0)
                {
                    delete mBase;
                    delete mCount;
                }
            }

            mBase = other.mBase;
            mCount = other.mCount;
            
            if (other.mCount)
            {
                *mCount += 1;
            }

            return *this;
        }

        T* get()
        {
            return mBase;
        }

        T& operator*()
        {
            return *mBase;
        }

        T* operator->()
        {
            return mBase;
        }

        operator bool()
        {
            return mBase != nullptr;
        }

        bool operator==(const shared_ptr& other) const
        {
            return (mBase == other.mBase);
        }

    private:
        T* mBase;
        atomic_long* mCount;
    };

    template <class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        T* object = new T(args...);
        return shared_ptr<T>(object);
    }
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_PTR_H