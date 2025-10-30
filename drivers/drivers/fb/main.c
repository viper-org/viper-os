#include <driver.h>

#include <stdint.h>
#include <string.h>

ssize_t read(void* buf, size_t* count, size_t seek);
ssize_t write(const void* buf, size_t count, size_t seek);
int stat(struct stat *statbuf);
int ioctl(unsigned long op, void *argp);

struct framebuffer {
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
};

__attribute__((section(".driver_header"))) volatile DriverHeader header = {
    .read = read,
    .write = write,
    .stat = stat,
    .ioctl = ioctl,
    .name = "fb"
};

KERNEL_FUNC(void, KeDebugLog, const char *);
KERNEL_FUNC(void, KeDebugLogFmt, const char *, ...);
KERNEL_FUNC(int, KeDrvGetFramebuffer, struct framebuffer *);

struct framebuffer fb;

void _start(void)
{
    GET_KERNEL_FUNC(void, KeDebugLog, const char*);
    GET_KERNEL_FUNC(void, KeDebugLogFmt, const char*, ...);
    GET_KERNEL_FUNC(int, KeDrvGetFramebuffer, struct framebuffer *);
    KeDrvGetFramebuffer(&fb);

    KeDebugLog("Initialized framebuffer driver\n");
}

ssize_t read(void *buf, size_t* count, size_t seek)
{
    (void)buf;
    (void)seek;
    *count = 0;
    // Can't read from fb
    return -1;
}

ssize_t write(const void *buf, size_t count, size_t seek)
{
    (void)seek;
    if (count != fb.height * fb.pitch) return -1;

    memcpy(fb.address, buf, count);
    return 0;
}

int stat(struct stat *statbuf)
{
    statbuf->st_size = fb.height * fb.pitch;
    return 0;
}

enum fb_ioctl
{
    FB_GET_HORIZ=0,
    FB_GET_VERT,
    FB_GET_PITCH
};

int ioctl(unsigned long op, void *argp)
{
    switch (op)
    {
        case FB_GET_HORIZ:
        {
            uint64_t *loc = argp;
            *loc = fb.width;
            break;
        }
        
        case FB_GET_VERT:
        {
            uint64_t *loc = argp;
            *loc = fb.height;
            break;
        }

        case FB_GET_PITCH:
        {
            uint64_t *loc = argp;
            *loc = fb.pitch;
            break;
        }

        default:
            return -1;
    }
    return 0;
}
