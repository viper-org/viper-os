#ifndef VIPEROS_SYSCALL_SYSCALLS
#define VIPEROS_SYSCALL_SYSCALLS 1

#include <stdint.h>

ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
int sys_open(const char *path, uint16_t openmode);
long sys_lseek(int fd, long offset, int whence);


void sys_yield(void);

#endif // VIPEROS_SYSCALL_SYSCALLS
