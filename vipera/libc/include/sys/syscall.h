#ifndef VIPEROS_VIPERA_LIBC_SYS_SYSCALL_H
#define VIPEROS_VIPERA_LIBC_SYS_SYSCALL_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/unistd.h>

#define SYS_read 0
#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_ioctl 4
#define SYS_mmap 5
#define SYS_seek 6
#define SYS_pipe 7
#define SYS_spawn 8
#define SYS_poll 9
#define SYS_signal 10
#define SYS_raise 11
#define SYS_exit 12
#define SYS_wait 13
#define SYS_movfd 14

#ifdef __cplusplus
}
#endif

#endif // VIPEROS_VIPERA_LIBC_SYS_SYSCALL_H