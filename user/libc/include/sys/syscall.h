#ifndef VIPEROS_LIBC_SYS_SYSCALL
#define VIPEROS_LIBC_SYS_SYSCALL 1

typedef unsigned long uint64_t;

uint64_t syscall0(uint64_t n);
uint64_t syscall1(uint64_t n, uint64_t arg0);
uint64_t syscall2(uint64_t n, uint64_t arg0, uint64_t arg1);
uint64_t syscall3(uint64_t n, uint64_t arg0, uint64_t arg1, uint64_t arg2);
uint64_t syscall4(uint64_t n, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3);
uint64_t syscall5(uint64_t n, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
uint64_t syscall6(uint64_t n, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

#define SYS_read 0
#define SYS_write 1
#define SYS_open 2
#define SYS_close 3
#define SYS_stat 4
#define SYS_fstat 5
#define SYS_poll1 7
#define SYS_lseek 8
#define SYS_mmap 9
#define SYS_munmap 11
#define SYS_waitpid 12
#define SYS_getpid 13
#define SYS_getppid 14
#define SYS_ioctl 16
#define SYS_poll 17
#define SYS_sigaction 18
#define SYS_kill 19
#define SYS_raise 20
#define SYS_pipe 22
#define SYS_yield 24
#define SYS_spawn 25
#define SYS_dup 33
#define SYS_dup2 34
#define SYS_loadlib 59
#define SYS_exit 60
#define SYS_uname 63
#define SYS_getcwd 79
#define SYS_chdir 80
#define SYS_mkdir 83

#endif // VIPEROS_LIBC_SYS_SYSCALL
