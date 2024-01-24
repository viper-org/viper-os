#include <sys/mman.h>
#include <sys/syscall.h>

void *mmap(size_t length)
{
    return (void *)syscall1(SYS_mmap, length);
}