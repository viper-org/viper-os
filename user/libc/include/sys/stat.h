#ifndef VIPEROS_LIBC_SYS_STAT
#define VIPEROS_LIBC_SYS_STAT 1

typedef unsigned long size_t;

struct stat
{
    size_t st_size;
};

int stat(const char *restrict path, struct stat *restrict statbuf);
int fstat(int fd, struct stat *statbuf);

#endif // VIPEROS_LIBC_SYS_STAT
