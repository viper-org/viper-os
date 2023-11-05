#include <driver/framebufferInstall.h>

#include <echis/driver/framebuffer.h>

#include <limine.h>

namespace x64
{
    static volatile limine_framebuffer_request framebufferRequest = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 1,
        .response = nullptr
    };
    
    int InstallFramebuffer()
    {
        if (limine_framebuffer_response* response = framebufferRequest.response)
        {
            if (response->framebuffer_count == 0)
            {
                return 1;
            }

            if (limine_framebuffer* framebuffer = response->framebuffers[0])
            {
                echis::framebuffer::Init(framebuffer->address,
                                             framebuffer->pitch,
                                             framebuffer->width,
                                             framebuffer->height);
                return 0;
            }
        }
        return 1;
    }
}