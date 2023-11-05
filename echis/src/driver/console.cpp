#include <driver/console.h>
#include <driver/framebuffer.h>

namespace echis
{
    console::FontInfo console::globalFontInfo;
    uint32_t console::mHoriz;
    uint32_t console::mVert;
    uint32_t console::mX;
    uint32_t console::mY;

    void console::Init(FontInfo newFontInfo, uint32_t screenHoriz, uint32_t screenVert, FontInfo& fontInfo)
    {
        if (fontInfo.bitmapData == nullptr)
        {
            fontInfo = newFontInfo;
        }
    }


#ifdef VIPEROS_ENABLE_TEST_FUNCTIONS
    void console::ResetGlobalState()
    {
        globalFontInfo = {
            .bitmapData = nullptr,
            .sizeX      = 0,
            .sizeY      = 0,
        };
    }
#endif
}