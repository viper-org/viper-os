#include "driver.h"

ssize_t read(void* buf, size_t count);
ssize_t write(const void* buf, size_t count);

__attribute__((section(".driver_header"))) volatile DriverHeader header = {
    .read = read,
    .write = write,
    .name = "sample"
};

KERNEL_FUNC(void, KeDebugLog, const char*);

void _start(void)
{
    KeDebugLog = (void(*)(const char*))(header.kernel_func_getter("KeDebugLog"));
    GET_KERNEL_FUNC(void, KeDebugLog, const char*);

    KeDebugLog("driver test");
}

ssize_t read(void* buf, size_t count)
{
    (void)buf;
    (void)count;
    KeDebugLog("Reading");
    return 0;
}

ssize_t write(const void* buf, size_t count)
{
    (void)buf;
    (void)count;
    KeDebugLog("Writing");
    return 0;
}
