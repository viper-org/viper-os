#ifndef VIPEROS_ECHIS_UTIL_MATH_H
#define VIPEROS_ECHIS_UTIL_MATH_H 1

#include <stdint.h>

namespace echis
{
    namespace util
    {
        template <typename T>
        static inline T AlignUp(T val, int align)
        {
            return reinterpret_cast<T>((reinterpret_cast<intmax_t>(val) & ~(align - 1)) + align);
        }
    }
}

#endif