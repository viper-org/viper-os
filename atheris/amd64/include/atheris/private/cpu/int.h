#ifndef VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_CPU_INT_H
#define VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_CPU_INT_H 1

namespace atheris
{
    namespace cpu
    {
        void PushDisableInt();
        void PopDisableInt();

        void DisableInt();
    }
}

#endif // VIPEROS_ATHERIS_AMD64_ATHERIS_PRIVATE_CPU_INT_H