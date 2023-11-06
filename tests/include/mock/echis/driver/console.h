#ifndef VIPEROS_MOCK_ECHIS_DRIVER_CONSOLE_H
#define VIPEROS_MOCK_ECHIS_DRIVER_CONSOLE_H 1

#include <stdint.h>
#include <vector>

namespace mock
{
    namespace echis
    {
        struct consoleImpl
        {
            struct PutCharAtInvocation
            {
                unsigned char c;
                uint32_t x;
                uint32_t y;
                uint32_t foreground;
                uint32_t background;
            };
            static std::vector<PutCharAtInvocation> PutCharAtInvocations;
            static void PutCharAt(unsigned char c, uint32_t x, uint32_t y, uint32_t foreground, uint32_t background);


            struct PutCharInvocation
            {
                unsigned char c;
                uint32_t foreground;
                uint32_t background;
            };
            static std::vector<PutCharInvocation> PutCharInvocations;
            static void PutChar(unsigned char c, uint32_t foreground, uint32_t background);
            

            struct PutCharMoveCursorInvocation
            {
            };
            static std::vector<PutCharMoveCursorInvocation> PutCharMoveCursorInvocations;
            static void PutCharMoveCursor();
        };
    }
}

#endif // VIPEROS_MOCK_ECHIS_DRIVER_CONSOLE_H