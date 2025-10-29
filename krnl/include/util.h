#ifndef VIPEROS_UTIL
#define VIPEROS_UTIL

static inline int align_up(int v, int align)
{
    return (v & ~(align - 1)) + align;
}

#endif // VIPEROS_UTIL
