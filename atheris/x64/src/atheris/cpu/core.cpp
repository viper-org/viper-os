#include <atheris/private/cpu/core.h>

#include <cpu/asm.h>

namespace atheris
{
    namespace cpu
    {
        namespace core
        {
            CoreLocal::CoreLocal(int id)
                : self(this)
                , id(id)
            {
            }

            CoreLocal* CoreLocal::Get()
            {
                return reinterpret_cast<CoreLocal*>(x64::cpu::ReadGS());
            }

            CoreID::operator int()
            {
                return CoreLocal::Get()->id;
            }

            CoreID id;
        }
    }
}