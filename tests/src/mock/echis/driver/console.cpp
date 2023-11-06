#include <mock/echis/driver/console.h>

namespace mock
{
    namespace echis
    {
        std::vector<consoleImpl::PutCharAtInvocation>         consoleImpl::PutCharAtInvocations;
        std::vector<consoleImpl::PutCharInvocation>           consoleImpl::PutCharInvocations;
        std::vector<consoleImpl::PutCharMoveCursorInvocation> consoleImpl::PutCharMoveCursorInvocations;

        void consoleImpl::PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background)
        {
            PutCharAtInvocations.push_back({
                c, x, y, foreground, background
            });
        }

        void consoleImpl::PutChar(unsigned char c, uint32_t foreground, uint32_t background)
        {
            PutCharInvocations.push_back({
                c, foreground, background
            });
        }

        void consoleImpl::PutCharMoveCursor()
        {
            PutCharMoveCursorInvocations.push_back({});
        }
    }
}