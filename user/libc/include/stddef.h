#ifndef VIPEROS_LIBC_STDDEF
#define VIPEROS_LIBC_STDDEF 1

typedef unsigned long ptrdiff_t;
typedef struct __max_align_t_internal { long double __max_align_t_a; long long __max_align_t_b; } max_align_t;
typedef unsigned long size_t;

#define NULL 0

// todo: offsetof()

#endif // VIPEROS_LIBC_STDDEF
