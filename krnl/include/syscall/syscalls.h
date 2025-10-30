#ifndef VIPEROS_SYSCALL_SYSCALLS
#define VIPEROS_SYSCALL_SYSCALLS 1

#include "syscall/stat.h"

#include <stdint.h>

ssize_t sys_read(int fd, void *buf, size_t count);
ssize_t sys_write(int fd, const void *buf, size_t count);
int sys_open(const char *path, uint16_t openmode);
int sys_close(int fd);
int sys_stat(const char *path, struct stat *statbuf);
int sys_fstat(int fd, struct stat *statbuf);
// lstat
long sys_lseek(int fd, long offset, int whence);
int sys_pipe(int fds[2]);


void sys_yield(void);
void sys_exit(int code);
int sys_waitpid(int pid, int *status, int options);
int sys_getpid(void);

#endif // VIPEROS_SYSCALL_SYSCALLS
