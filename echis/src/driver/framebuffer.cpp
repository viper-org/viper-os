#include <driver/framebuffer.h>
#include <driver/impl/framebuffer.h>

#include <fs/devfs.h>

#include <string.h>

namespace echis
{
    namespace framebuffer
    {
        void Init(void* base, uint64_t pitch, uint64_t horiz, uint64_t vert)
        {
            framebufferImpl::Init(base, pitch, horiz, vert);
        }

        void PutPixel(uint32_t x, uint32_t y, uint32_t color)
        {
            framebufferImpl::PutPixel(x, y, color);
        }

        void DrawBitmap(void* bitmap, uint32_t x, uint32_t y, uint32_t sizeX, uint32_t sizeY, uint32_t foreground, uint32_t background)
        {
            framebufferImpl::DrawBitmap(bitmap, x, y, sizeX, sizeY, foreground, background);
        }


        int DeviceRead(void*, size_t* count)
        {
            *count = 0;
            return -1;
        }

        int DeviceWrite(const void* buffer, size_t count)
        {
            if (count != framebufferImpl::GetVert() * framebufferImpl::GetPitch())
            {
                return -1;
            }

            memcpy(framebufferImpl::GetBase(), buffer, count);
            return 0;
        }

        int DeviceIoctl(unsigned long request, char* arg)
        {
            switch(request)
            {
                case 0: // horiz request
                {
                    uint64_t* location = reinterpret_cast<uint64_t*>(arg);
                    *location = framebufferImpl::GetHoriz();
                    break;
                }
                case 1: // vert request
                {
                    uint64_t* location = reinterpret_cast<uint64_t*>(arg);
                    *location = framebufferImpl::GetVert();
                    break;
                }
                case 2: // pitch request
                {
                    uint64_t* location = reinterpret_cast<uint64_t*>(arg);
                    *location = framebufferImpl::GetPitch();
                    break;
                }
                default:
                    return -1;
            }
            return 0;
        }

        void SetupDeviceFile()
        {
            fs::devfs::AddDeviceFile("fb", DeviceRead, DeviceWrite, DeviceIoctl);
        }
    }
}