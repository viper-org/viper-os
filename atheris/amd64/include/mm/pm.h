#ifndef VIPEROS_ATHERIS_AMD64_MM_PM_H
#define VIPEROS_ATHERIS_AMD64_MM_PM_H 1

#include <limine.h>

namespace atheris
{
    namespace amd64
    {
        namespace pm
        {
            void Init();

            limine_memmap_response* GetMemoryMap();
        }
    }
}

#endif // VIPEROS_ATHERIS_AMD64_MM_PM_H