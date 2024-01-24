#include <sys/unistd.h>
#include <sys/syscall.h>

off_t lseek(int fd, off_t offset, int whence)
{
    if (whence != SEEK_SET) return -1;
    
    return syscall3(SYS_seek, fd, offset, whence);
}