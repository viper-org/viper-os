#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>

void _start(void)
{
    spawn("/bin/usertest2");
    syscall1(67, (uint64_t)"TEST");

    unsigned long pitch;
    unsigned long height;
    
    int fd = open("/dev/fb", 3);
    ioctl(fd, 1, &height);
    ioctl(fd, 2, &pitch);

    void *mem = mmap(0, pitch * height, 0, 0, 0, 0);
    for (unsigned long i = 0; i < pitch * height; ++i)
    {
        ((char*)mem)[i] = 0x87;
    }
    write(fd, mem, pitch * height);

    while (1) __asm__ volatile("pause");
}
