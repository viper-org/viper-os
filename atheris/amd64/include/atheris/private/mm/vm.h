#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H 1

#include <atheris/mm/pm.h>

namespace atheris
{
    namespace vm
    {
        void* GetVirtualAddress(pm::physaddr physAddress);
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_MM_VM_H