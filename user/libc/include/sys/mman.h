#ifndef VIPEROS_LIBC_SYS_MMAN
#define VIPEROS_LIBC_SYS_MMAN 1

typedef unsigned long size_t;
typedef long off_t;

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

#endif // VIPEROS_LIBC_SYS_MMAN
