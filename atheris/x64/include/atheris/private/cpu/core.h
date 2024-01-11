#ifndef VIPEROS_ATHERIS_X64_ATHERIS_CPU_CORE_H
#define VIPEROS_ATHERIS_X64_ATHERIS_CPU_CORE_H 1

#include <cpu/gdt/tss.h>

namespace atheris
{
    namespace cpu
    {
        namespace core
        {
            struct CoreLocal
            {
                CoreLocal(int id);

                CoreLocal* self;
                int id;
                ::x64::cpu::tss::TSSDescriptor tssDesc;

                static CoreLocal* Get();
            };

            struct CoreID
            {
                operator int();
            };

            extern CoreID id;
        }
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_CPU_CORE_H