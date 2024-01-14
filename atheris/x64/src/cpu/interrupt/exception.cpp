#include <cpu/context.h>

#include <mm/vm.h>

#include <echis/core/exception/exception.h>

namespace atheris
{
    namespace x64
    {
        extern "C" void CommonExceptionHandler(cpu::Context* context)
        {
            echis::exception::Exception exceptionType;
            switch(context->BaseFrame.vector)
            {
                case 0:
                    exceptionType = echis::exception::DIV;
                    break;
                case 14:
                    if (!atheris::vm::HandlePageFault(context))
                    {
                        return;
                    }
                    exceptionType = echis::exception::SEG;
                    break;
                case 6:
                    exceptionType = echis::exception::ILL;
                    break;
                case 1:
                case 3:
                    exceptionType = echis::exception::BRK;
                    break;
                case 2:
                case 4:
                case 5:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 28:
                case 29:
                case 30:
                    exceptionType = echis::exception::ARC;
            }
            echis::exception::raise(exceptionType, context->BaseFrame.error_code);
        }
    }
}