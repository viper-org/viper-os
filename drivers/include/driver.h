#ifndef VIPEROS_DRIVERS_DRIVER
#define VIPEROS_DRIVERS_DRIVER 1

typedef unsigned long size_t;
typedef long ssize_t;

typedef struct __attribute__((packed))
{
    void*(*kernel_func_getter)(const char*);

    ssize_t (*read)(void* buf, size_t *count, size_t seek);
    ssize_t (*write)(const void* buf, size_t count, size_t seek);
    int (*ioctl)(unsigned long op, void *argp);

    const char* name;
} DriverHeader;

#define KERNEL_FUNC(retty, name, ...) retty(*name)(__VA_ARGS__)
#define GET_KERNEL_FUNC(retty, name, ...) (name = (retty(*)(__VA_ARGS__))(header.kernel_func_getter(#name)))

#endif // VIPEROS_DRIVERS_DRIVER
