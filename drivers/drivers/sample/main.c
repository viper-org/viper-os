#include <driver.h>

ssize_t read(void* buf, size_t count);
ssize_t write(const void* buf, size_t count);
int ioctl(unsigned long op, void *argp);

__attribute__((section(".driver_header"))) volatile DriverHeader header = {
    .read = read,
    .write = write,
    .ioctl = ioctl,
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

ssize_t read(void* buf, size_t count)
{
    (void)buf;
    (void)count;
    KeDebugLogFmt("Reading %d bytes into %p\n", count, buf);
    return 0;
}

ssize_t write(const void* buf, size_t count)
{
    (void)buf;
    (void)count;
    KeDebugLogFmt("Writing %d bytes to %p\n", count, buf);
    return 0;
}

int ioctl(unsigned long op, void *argp)
{
    KeDebugLogFmt("\nioctl called with op=%d", op);
    return 0;
}
