#ifndef VIPEROS_ATHERIS_X64_ATHERIS_CPU_SMP_INIT_H
#define VIPEROS_ATHERIS_X64_ATHERIS_CPU_SMP_INIT_H 1

namespace atheris
{
    namespace cpu
    {
        namespace smp
        {
            void Init();

            enum class IPIDestination
            {
                Single,
                Self,
                BroadcastAll,
                BroadcastOthers
            };

            namespace IPI
            {
                enum IPI
                {
                    Panic = 0xff,
                };
            }

            void SendIPI(int core, int vector, IPIDestination destination);
        };
    }
}

#endif // VIPEROS_ATHERIS_X64_ATHERIS_CPU_SMP_INIT_H