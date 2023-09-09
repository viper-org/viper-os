#ifndef VIPER_OS_CONTAINER_PTR_H
#define VIPER_OS_CONTAINER_PTR_H 1

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

        
        ~unique_ptr()
        {
            if (mData)
            {
                delete mData;
            }
        }


        T* get()
        {
            return mData;
        }
        T* release()
        {
            T* result = mData;
            mData = nullptr;
            return result;
        }


        T operator*()
        {
            return *mData;
        }

        T* operator->()
        {
            return mData;
        }

    private:
        T* mData;
    };

    template <class Tp, class... Args>
    unique_ptr<Tp> make_unique(Args&&... args)
    {
        return unique_ptr<Tp>(new Tp(args...));
    }
}

#endif // VIPER_OS_CONTAINER_PTR_H