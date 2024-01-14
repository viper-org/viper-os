#include <atheris/private/fs/initrd.h>

#include <fs/module.h>

namespace atheris
{
    namespace fs
    {
        namespace initrd
        {
            void* GetAddress()
            {
                x64::fs::Module fontModule = x64::fs::Module::Get("/initrd.tar");

                return fontModule.getAddress();
            }
        }
    }
}