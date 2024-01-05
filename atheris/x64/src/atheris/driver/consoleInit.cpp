#include <atheris/private/driver/consoleInit.h>

#include <fs/module.h>

#include <echis/driver/console.h>

#include <limine.h>

namespace atheris
{
    namespace framebuffer
    {
        extern limine_framebuffer_response* response;
    }

    namespace console
    {
        using namespace x64;

        void Init()
        {
            fs::Module fontModule = fs::Module::Get("/font.bmp");

            echis::console::Init({fontModule.getAddress(), 8, 8},
                                framebuffer::response->framebuffers[0]->width, framebuffer::response->framebuffers[0]->height);
        }
    }
}