#ifndef VIPEROS_LIBC_POLL
#define VIPEROS_LIBC_POLL 1

typedef unsigned long size_t;

int poll1(int fd);
int poll(int *fds, size_t nfds);

#endif // VIPEROS_LIBC_POLL
