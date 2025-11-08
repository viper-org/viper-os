#ifndef VIPEROS_LIBC_SYS_WAIT
#define VIPEROS_LIBC_SYS_WAIT 1

typedef int pid_t;

pid_t waitpid(pid_t pid, int *status, int options);

#endif // VIPEROS_LIBC_SYS_WAIT
