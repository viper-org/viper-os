#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_STDARG_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_STDARG_H 1

#ifdef __cplusplus
extern "C" {
#endif

typedef __builtin_va_list va_list;

#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_copy __builtin_va_copy
#define va_end __builtin_va_end

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_STDARG_H