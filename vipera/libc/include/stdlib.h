#ifndef VIPEROS_VIPERA_LIBC_STDLIB_H
#define VIPEROS_VIPERA_LIBC_STDLIB_H 1

#ifdef __cplusplus
extern "C" {
#endif

#ifdef VIPEROS_VIPERA_LIBCPP_CSTDLIB
namespace std {
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

_Noreturn void abort(void);
_Noreturn void exit(int exit_code);
_Noreturn void quick_exit(int exit_code);
_Noreturn void _Exit(int exit_code);

int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));

#ifdef VIPEROS_VIPERA_LIBCPP_CSTDLIB
}
#endif

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_STDLIB_H