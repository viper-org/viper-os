#include <cpu/cpu.h>

#include <echis/driver/debugcon.h>

namespace atheris
{
    namespace amd64
    {
        namespace cpu
        {
            extern "C" void CommonExceptionHandler(State* cpuState)
            {
                echis::driver::debugcon::WriteFormatted("Recieved exception: %d\n", cpuState->BaseFrame.vector);
                for(;;) asm("pause");
                // TODO: Call generic exception handler in echis with an arch-independent exception type
            }
        }
    }
}