#ifndef VIPEROS_STDARG
#define VIPEROS_STDARG 1

#define va_start __builtin_va_start
#define va_arg __builtin_va_arg
#define va_end __builtin_va_end
#define va_copy __builtin_va_copy

#define va_list __builtin_va_list

#endif // VIPEROS_STDARG
