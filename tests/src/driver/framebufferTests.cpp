#include <echis/driver/framebuffer.h>

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <random>

namespace framebufferTests
{
    constexpr int rowSize   = 320;
    constexpr int colSize   = 200;
    constexpr int pixelSize = 4;
    constexpr int pitch     = rowSize * pixelSize;

    class FramebufferTestsFixture
    {
    public:
        FramebufferTestsFixture()
            : mFramebufferBase(std::make_unique<std::byte[]>(pitch * colSize))
        {
            echis::framebuffer::Init(mFramebufferBase.get(), pitch, rowSize, colSize);
        }

    protected:
        std::unique_ptr<std::byte[]> mFramebufferBase;
    };

    TEST_CASE_METHOD(FramebufferTestsFixture, "Putting a pixel at (0,0)", "[framebufferTests]")
    {
        std::mt19937 rng;
        std::uniform_int_distribution<int> colorUid(0, 0xff);

        int r = colorUid(rng);
        int g = colorUid(rng);
        int b = colorUid(rng);

        echis::framebuffer::PutPixel(0, 0, r << 16 | g << 8 | b);

        uint8_t* base = reinterpret_cast<uint8_t*>(mFramebufferBase.get());

        REQUIRE(base[0] == b);
        REQUIRE(base[1] == g);
        REQUIRE(base[2] == r);
    }

    TEST_CASE_METHOD(FramebufferTestsFixture, "Putting pixels on the first row", "[framebufferTests]")
    {
        for (int i = 0; i < 100; i++)
        {
            std::mt19937 rng;
            std::uniform_int_distribution<int> uid(0, rowSize);
            std::uniform_int_distribution<int> colorUid(0, 0xff);

            int x = uid(rng);

            int r = colorUid(rng);
            int g = colorUid(rng);
            int b = colorUid(rng);

            echis::framebuffer::PutPixel(x, 0, r << 16 | g << 8 | b);

            uint8_t* base = reinterpret_cast<uint8_t*>(mFramebufferBase.get());

            REQUIRE(base[x * pixelSize + 0] == b);
            REQUIRE(base[x * pixelSize + 1] == g);
            REQUIRE(base[x * pixelSize + 2] == r);
        }
    }
    
    TEST_CASE_METHOD(FramebufferTestsFixture, "Putting pixels anywhere", "[framebufferTests]")
    {
        for (int i = 0; i < 100; i++)
        {
            std::mt19937 rng;
            std::uniform_int_distribution<int> uid(0, rowSize);
            std::uniform_int_distribution<int> colorUid(0, 0xff);

            int x = uid(rng);
            int y = uid(rng);

            int r = colorUid(rng);
            int g = colorUid(rng);
            int b = colorUid(rng);

            echis::framebuffer::PutPixel(x, y, r << 16 | g << 8 | b);

            uint8_t* base = reinterpret_cast<uint8_t*>(mFramebufferBase.get());

            REQUIRE(base[x * pixelSize + y * pitch + 0] == b);
            REQUIRE(base[x * pixelSize + y * pitch + 1] == g);
            REQUIRE(base[x * pixelSize + y * pitch + 2] == r);
        }
    }
}