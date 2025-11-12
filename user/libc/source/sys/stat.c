#include <sys/stat.h>
#include <sys/syscall.h>

int stat(const char *restrict path, struct stat *restrict statbuf)
{
    return syscall2(SYS_stat, (uint64_t)path, (uint64_t)statbuf);
}

int fstat(int fd, struct stat *restrict statbuf)
{
    return syscall2(SYS_fstat, fd, (uint64_t)statbuf);
}
