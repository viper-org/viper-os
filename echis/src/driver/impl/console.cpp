#include <driver/impl/console.h>
#include <driver/impl/framebuffer.h>

namespace echis
{
    consoleImpl::FontInfo consoleImpl::globalFontInfo;
    uint32_t consoleImpl::mHoriz;
    uint32_t consoleImpl::mVert;
    uint32_t consoleImpl::mX;
    uint32_t consoleImpl::mY;

    void consoleImpl::Init(FontInfo newFontInfo, uint32_t screenHoriz, uint32_t screenVert, FontInfo& fontInfo)
    {
        if (fontInfo.bitmapData == nullptr)
        {
            fontInfo = newFontInfo;
            mHoriz = screenHoriz;
            mVert = screenVert;
        }
    }


#ifdef VIPEROS_ENABLE_TEST_FUNCTIONS
    void consoleImpl::ResetGlobalState()
    {
        globalFontInfo = {
            .bitmapData = nullptr,
            .sizeX      = 0,
            .sizeY      = 0,
        };
    }
#endif
}