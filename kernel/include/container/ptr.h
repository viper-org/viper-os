#ifndef VIPER_OS_CONTAINER_PTR_H
#define VIPER_OS_CONTAINER_PTR_H 1

#include <drivers//Terminal.h>

#include <memory.h>

#include <cstddef>

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


        T operator*() const
        {
            return *mData;
        }

        T* operator->() const
        {
            return mData;
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
}

#endif // VIPER_OS_CONTAINER_PTR_H