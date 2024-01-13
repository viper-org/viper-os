#ifndef VIPEROS_ECHIS_STD_CONTAINER_LATE_INIT_H
#define VIPEROS_ECHIS_STD_CONTAINER_LATE_INIT_H 1

#include <new>

namespace vpr
{
    template <class T>
    class late_init
    {
    public:
        late_init()
        {
        }

        late_init(const T& obj)
        {
            new(mStorage) T(obj);
        }

        late_init& operator=(const T& obj)
        {
            new(mStorage) T(obj);
            return *this;
        }

        operator T&()
        {
            return *reinterpret_cast<T*>(&mStorage);
        }

        T& operator*()
        {
            return *reinterpret_cast<T*>(&mStorage);
        }

        T* operator&()
        {
            return reinterpret_cast<T*>(&mStorage);
        }

        T* operator->()
        {
            return reinterpret_cast<T*>(&mStorage);
        }

    private:
        alignas(T) char mStorage[sizeof(T)] {0};
    };
}

#endif // VIPEROS_ECHIS_STD_CONTAINER_LATE_INIT_H