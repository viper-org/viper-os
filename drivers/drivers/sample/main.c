#include <driver.h>

ssize_t read(void* buf, size_t *count, size_t seek);
ssize_t write(const void* buf, size_t count, size_t seek);
int stat(struct stat *statbuf);
int ioctl(unsigned long op, void *argp);
void mmap(void *, void *, size_t);

__attribute__((section(".driver_header"))) volatile DriverHeader header = {
    .read = read,
    .write = write,
    .stat = stat,
    .ioctl = ioctl,
    .mmap = mmap,
    .name = "sample"
};

KERNEL_FUNC(void, KeDebugLog, const char*);
KERNEL_FUNC(void, KeDebugLogFmt, const char*, ...);

void _start(void)
{
    GET_KERNEL_FUNC(void, KeDebugLog, const char*);
    GET_KERNEL_FUNC(void, KeDebugLogFmt, const char*, ...);

    KeDebugLog("Initialized sample driver\n");
}

ssize_t read(void* buf, size_t* count, size_t seek)
{
    (void)buf;
    (void)count;
    (void)seek;
    KeDebugLogFmt("Reading %d bytes into %p\n", *count, buf);
    return 0;
}

ssize_t write(const void* buf, size_t count, size_t seek)
{
    (void)buf;
    (void)count;
    (void)seek;
    KeDebugLogFmt("Writing %d bytes to %p\n", count, buf);
    return 0;
}

int stat(struct stat *statbuf)
{
    (void)statbuf;
    KeDebugLogFmt("Stat called\n");
    return 0;
}

int ioctl(unsigned long op, void *argp)
{
    (void)argp;
    KeDebugLogFmt("\nioctl called with op=%d", op);
    return 0;
}

void mmap(void *a, void *m, size_t s)
{
}
