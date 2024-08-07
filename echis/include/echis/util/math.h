#ifndef VIPEROS_ECHIS_UTIL_MATH_H
#define VIPEROS_ECHIS_UTIL_MATH_H 1

#include <cstdint>

namespace echis
{
    namespace util
    {
        template <typename T>
        constexpr static inline T AlignUp(T val, int align)
        {
            return reinterpret_cast<T>((reinterpret_cast<std::uintmax_t>(val) & ~(align - 1)) + align);
        }
    }
}

#endif // VIPEROS_ECHIS_UTIL_MATH_H