#ifndef VIPEROS_ECHIS_STD_UTILITY_H
#define VIPEROS_ECHIS_STD_UTILITY_H 1

namespace vpr
{
    template <class T> struct remove_reference { typedef T type; };
    template <class T> struct remove_reference<T&> { typedef T type; };
    template <class T> struct remove_reference<T&&> { typedef T type; };

    template <class T> typename remove_reference<T>::type&& move(T&& t)
    {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }
}

#endif // VIPEROS_ECHIS_STD_UTILITY_H