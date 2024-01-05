#include <atheris/driver/framebufferInit.h>

#include <echis/driver/framebuffer.h>

#include <limine.h>

namespace atheris
{
    namespace framebuffer
    {
        static volatile limine_framebuffer_request framebufferRequest = {
            .id = LIMINE_FRAMEBUFFER_REQUEST,
            .revision = 1,
            .response = nullptr
        };

        limine_framebuffer_response* response;
        
        void Init()
        {
            limine_framebuffer* framebuffer = framebufferRequest.response->framebuffers[0];
            echis::framebuffer::Init(framebuffer->address,
                                        framebuffer->pitch,
                                        framebuffer->width,
                                        framebuffer->height);

            response = framebufferRequest.response;
        }
    }
}