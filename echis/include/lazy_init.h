#ifndef VIPEROS_ECHIS_LAZY_INIT_H
#define VIPEROS_ECHIS_LAZY_INIT_H 1

#include <new>
#include <utility>

template <class T>
class lazy_init
{
public:
    lazy_init()
    {
    }

    lazy_init(const T& v)
    {
        new (mBuffer) T(v);
    }

    lazy_init(T&& v)
    {
        new (mBuffer) T(std::move(v));
    }

    void default_init()
    {
        new (mBuffer) T;
    }

    T& operator*()
    {
        return *reinterpret_cast<T*>(mBuffer);
    }

    T* operator&()
    {
        return reinterpret_cast<T*>(mBuffer);
    }

    T* operator->()
    {
        return reinterpret_cast<T*>(mBuffer);
    }

private:
    alignas(T) char mBuffer[sizeof(T)];
};

#endif // VIPEROS_ECHIS_LAZY_INIT_H