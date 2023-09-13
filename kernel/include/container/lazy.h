#ifndef VIPER_OS_CONTAINER_LAZY_H
#define VIPER_OS_CONTAINER_LAZY_H 1

#include <new>

namespace vpr
{
    template <class T>
    class lazy
    {
    public:
        lazy()
        {
        }

        lazy(T object)
        {
            new(mBuffer)T(object);
        }


        void operator=(const T& object)
        {
            new(mBuffer)T(object);
        }


        T& operator*()
        {
            return *((T*)&mBuffer);
        }

        T* operator->()
        {
            return (T*)&mBuffer[0];
        }

    private:
        alignas(T) char mBuffer[sizeof(T)] {0};
    };
}

#endif