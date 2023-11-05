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

    limine_framebuffer_response* framebufferResponse;
    
    int InstallFramebuffer()
    {
        if ((framebufferResponse = framebufferRequest.response))
        {
            if (framebufferResponse->framebuffer_count == 0)
            {
                return 1;
            }

            if (limine_framebuffer* framebuffer = framebufferResponse->framebuffers[0])
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