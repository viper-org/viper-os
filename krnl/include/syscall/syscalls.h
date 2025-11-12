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
int sys_poll1(int fd);
long sys_lseek(int fd, long offset, int whence);
int sys_ioctl(int fd, unsigned long op, char *argp);
int sys_poll(int *fds, size_t nfds);
int sys_pipe(int fds[2]);
int sys_dup(int fd);
int sys_dup2(int old, int new);

char *sys_getcwd(char buf[], size_t size);
int sys_chdir(char *path);
int sys_mkdir(const char *path); // todo: mode?


void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, long offset);
int sys_munmap(void *addr, size_t len);


void sys_yield(void);
void sys_exit(int code);
int sys_waitpid(int pid, int *status, int options);
int sys_getpid(void);
int sys_getppid(void);
int sys_spawn(const char *path, int argc, char **argv); // todo: argc, argv, envp

#endif // VIPEROS_SYSCALL_SYSCALLS
