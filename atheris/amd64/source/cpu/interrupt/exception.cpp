#include <cpu/cpu.h>

#include <echis/exception/exception.h>

#include <echis/driver/debugcon.h>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            extern "C" void CommonExceptionHandler(State* cpuState)
            {
                switch (cpuState->BaseFrame.vector)
                {
                    case 0xe:
                    {
                        echis::exception::AccessViolationException except(cpuState->ControlRegisters.cr2);
                        echis::exception::OnException(&except);
                        break; // echis will handle the exception or kill the active thread so returning is OK
                    }

                    default:
                        echis::driver::debugcon::WriteFormatted("Recieved unhandled exception: %d\n", cpuState->BaseFrame.vector);
                        for(;;) asm("pause");
                        break;
                }
            }
        }
    }
}