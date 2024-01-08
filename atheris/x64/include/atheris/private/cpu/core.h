#ifndef VIPEROS_ATHERIS_X64_ATHERIS_CPU_CORE_H
#define VIPEROS_ATHERIS_X64_ATHERIS_CPU_CORE_H 1

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