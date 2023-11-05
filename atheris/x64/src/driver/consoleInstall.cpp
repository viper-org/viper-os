#include <driver/consoleInstall.h>

#include <fs/module.h>

#include <echis/driver/console.h>

#include <limine.h>

namespace x64
{
    extern limine_framebuffer_response* framebufferResponse;

    int InstallConsole()
    {
        fs::Module fontModule = fs::Module::Get("/font.bmp");

        if (fontModule.getAddress() == nullptr)
        {
            return 1;
        }

        echis::console::Init({fontModule.getAddress(), 8, 8},
                             framebufferResponse->framebuffers[0]->width, framebufferResponse->framebuffers[0]->height);

        return 0;
    }
}