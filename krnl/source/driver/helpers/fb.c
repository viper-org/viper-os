#include "driver/driver.h"

#include <limine.h>

struct framebuffer {
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
};

static struct limine_framebuffer_request fbreq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 1,
    .response = NULL
};

DRIVER_EXPORT int KeDrvGetFramebuffer(struct framebuffer *out)
{
    if (!fbreq.response) return 1;
    if (!fbreq.response->framebuffer_count) return 1;

    // TODO: Get more framebuffers
    struct limine_framebuffer *fb = fbreq.response->framebuffers[0];

    *out = (struct framebuffer) {
        .address = fb->address,
        .width = fb->width,
        .height = fb->height,
        .pitch = fb->pitch,
        .bpp = fb->bpp
    };

    return 0;
}
