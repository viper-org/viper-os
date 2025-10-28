#ifndef VIPEROS_DRIVER_LDR_LOADER
#define VIPEROS_DRIVER_LDR_LOADER 1

#include <stdint.h>

typedef long ssize_t;

struct __attribute__((packed)) driver_header
{
    void* (*kernel_func_getter)(const char *);

    ssize_t (*read)(void *, size_t);
    ssize_t (*write)(const void *, size_t);

    const char *name;
};

struct driver
{
    struct driver_header *header;
    void (*init)(void);
};

void ldr_init(void);
struct driver ldr_load(void *addr);

#endif // VIPEROS_DRIVER_LDR_LOADER
