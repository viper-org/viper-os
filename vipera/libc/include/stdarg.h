#ifndef VIPEROS_VIPERA_LIBC_STDARG_H
#define VIPEROS_VIPERA_LIBC_STDARG_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef VIPEROS_VIPERA_LIBCPP_CSTDARG
namespace std {
#endif

typedef char* va_list;

#define va_start(ap, v) __builtin_va_start(ap, v)
#define va_arg(ap, t) __builtin_va_arg(ap, t)
#define va_end(ap) __builtin_va_end(ap)

#ifdef VIPEROS_VIPERA_LIBCPP_CSTDARG
}
#endif

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_STDARG_H