#include <mock/echis/driver/console.h>

namespace mock
{
    namespace echis
    {
        std::vector<console::PutCharAtInvocation>         console::PutCharAtInvocations;
        std::vector<console::PutCharInvocation>           console::PutCharInvocations;
        std::vector<console::PutCharMoveCursorInvocation> console::PutCharMoveCursorInvocations;

        void console::PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background)
        {
            PutCharAtInvocations.push_back({
                c, x, y, foreground, background
            });
        }

        void console::PutChar(unsigned char c, uint32_t foreground, uint32_t background)
        {
            PutCharInvocations.push_back({
                c, foreground, background
            });
        }

        void console::PutCharMoveCursor()
        {
            PutCharMoveCursorInvocations.push_back({});
        }
    }
}