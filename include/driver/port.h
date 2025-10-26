#ifndef VIPEROS_DRIVER_PORT
#define VIPEROS_DRIVER_PORT 1

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t r;
    __asm__ volatile("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}

static inline uint8_t inw(uint16_t port)
{
    uint16_t r;
    __asm__ volatile("inw %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}


static inline void outb(uint16_t port, uint8_t d)
{
    __asm__ volatile("outb %0, %1" :: "a"(d), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t d)
{
    __asm__ volatile("outw %0, %1" :: "a"(d), "Nd"(port));
}
#endif // VIPEROS_DRIVER_PORT
