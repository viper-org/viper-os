#ifndef VIPER_OS_CONTAINER_PTR_H
#define VIPER_OS_CONTAINER_PTR_H 1

#include <container/atomic.h>

#include <memory.h>

#include <cstddef>
#include <utility>

namespace vpr
{
    template <class T>
    class unique_ptr
    {
    public:
        unique_ptr()
            : mData(nullptr)
        {
        }

        unique_ptr(std::nullptr_t)
            : mData(nullptr)
        {
        }

        unique_ptr(T* ptr)
            : mData(ptr)
        {
        }

        template<typename U>
        unique_ptr(unique_ptr<U>&& other)
            : mData(other.release())
        {
        }

        template<typename U>
        unique_ptr& operator=(unique_ptr<U>&& other)
        {
            mData = std::move(other.mData);
            return *this;
        }

        
        ~unique_ptr()
        {
            if (mData)
            {
                delete mData;
            }
        }


        T* get() const
        {
            return mData;
        }
        T* release()
        {
            T* result = mData;
            mData = nullptr;
            return result;
        }


        T& operator*() const
        {
            return *mData;
        }

        T* operator->() const
        {
            return mData;
        }

        operator bool() const
        {
            return (mData != nullptr);
        }

    private:
        T* mData;
    };

    template <class T, class... Args>
    unique_ptr<T> make_unique(Args&&... args)
    {
        //return unique_ptr<T>(new Tp(args...));
        T* object = new T(args...);
        return unique_ptr<T>(object);
    }


    template <class T>
    class shared_ptr
    {
    public:
        shared_ptr()
            : mData(nullptr)
            , mRefCount(nullptr)
        {
        }

        shared_ptr(std::nullptr_t)
            : shared_ptr()
        {
        }

        shared_ptr(T* data)
            : mData(data)
            , mRefCount(new atomic_long(1))
        {
        }

        shared_ptr(const shared_ptr& other)
            : mData(other.mData)
            , mRefCount(other.mRefCount)
        {
            if (other.mRefCount)
            {
                *mRefCount += 1;
            }
        }

        ~shared_ptr()
        {
            if (mRefCount)
            {
                *mRefCount -= 1;
                if (*mRefCount == 0)
                {
                    delete mData;
                    delete mRefCount;
                }
            }
        }

        T* get()
        {
            return mData;
        }


        T& operator*()
        {
            return *mData;
        }

        T* operator->()
        {
            return mData;
        }


        long use_count() const
        {
            return *mRefCount;
        }

        bool unique() const
        {
            return (*mRefCount == 1);
        }

        operator bool()
        {
            return (mData != nullptr);
        }

        bool operator==(const shared_ptr other) const
        {
            return (mData == other.mData);
        }

    private:
        T* mData;
        atomic_long* mRefCount;
    };

    template <class T, class... Args>
    shared_ptr<T> make_shared(Args&&... args)
    {
        T* object = new T(args...);
        return shared_ptr<T>(object);
    }
}

#endif // VIPER_OS_CONTAINER_PTR_H