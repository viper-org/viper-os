#ifndef VIPER_OS_CONTAINER_ATOMIC_H
#define VIPER_OS_CONTAINER_ATOMIC_H 1

namespace vpr
{
    template <class T>
    class atomic
    {
    public:
        atomic() = default;

        constexpr atomic(T desired)
        {
            __atomic_store(&mData, &desired, __ATOMIC_RELAXED);
        }

        T operator=(T desired)
        {
            __atomic_store(&mData, &desired, __ATOMIC_RELAXED);

            T ret;
            __atomic_load(&mData, &ret, __ATOMIC_RELAXED);
            return ret;
        }

        operator T()
        {
            T ret;
            __atomic_load(&mData, &ret, __ATOMIC_RELAXED);
            return ret;
        }


        T operator++()
        {
            return __atomic_fetch_add(&mData, 1, __ATOMIC_RELAXED);
        }

        T operator++(int)
        {
            return __atomic_add_fetch(&mData, 1, __ATOMIC_RELAXED);
        }

        T operator--()
        {
            return __atomic_fetch_sub(&mData, 1, __ATOMIC_RELAXED);
        }

        T operator--(int)
        {
            return __atomic_sub_fetch(&mData, 1, __ATOMIC_RELAXED);
        }


        T operator+=(T arg)
        {
            return __atomic_fetch_add(&mData, arg, __ATOMIC_RELAXED);
        }

        T operator-=(T arg)
        {
            return __atomic_fetch_sub(&mData, arg, __ATOMIC_RELAXED);
        }

        T operator&=(T arg)
        {
            return __atomic_fetch_and(&mData, arg, __ATOMIC_RELAXED);
        }

        T operator|=(T arg)
        {
            return __atomic_fetch_or(&mData, arg, __ATOMIC_RELAXED);
        }

        T operator^=(T arg)
        {
            return __atomic_fetch_xor(&mData, arg, __ATOMIC_RELAXED);
        }

    private:
        T mData;
    };

    using atomic_bool       = atomic<bool>;
    using atomic_char       = atomic<char>;
    using atomic_uchar      = atomic<unsigned char>;
    using atomic_short      = atomic<short>;
    using atomic_ushort     = atomic<unsigned short>;
    using atomic_int        = atomic<int>;
    using atomic_uint       = atomic<unsigned int>;
    using atomic_long       = atomic<long>;
    using atomic_ulong      = atomic<unsigned long>;
    using atomic_long_long  = atomic<long long>;
    using atomic_ulong_long = atomic<unsigned long long>;
}

#endif // VIPER_OS_CONTAINER_ATOMIC_H