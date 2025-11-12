#include <sys/mman.h>
#include <sys/syscall.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    return (void *)syscall6(SYS_mmap, (uint64_t)addr, length, prot, flags, fd, offset);
}

int munmap(void *addr, size_t length)
{
    return syscall2(SYS_munmap, (uint64_t)addr, length);
}
