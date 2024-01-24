#ifndef VIPEROS_VIPERA_LIBC_SYS_UNISTD_H
#define VIPEROS_VIPERA_LIBC_SYS_UNISTD_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

long syscall0(long number);
long syscall1(long number, long arg0);
long syscall2(long number, long arg0, long arg1);
long syscall3(long number, long arg0, long arg1, long arg3);


int close(int);
_Noreturn void _exit(int);
off_t lseek(int, off_t, int);
int pipe(int[2]);
ssize_t read(int, void *, size_t);
ssize_t write(int, const void *, size_t);


#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_SYS_UNISTD_H