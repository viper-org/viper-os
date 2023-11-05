#include <echis/driver/console.h>

#include <iostream>
#include <mock/echis/driver/framebuffer.h>
#include <mock/echis/driver/console.h>

#include <catch2/catch_test_macros.hpp>

#include <random>

namespace consoleTests
{
    class ConsoleTestsFixture
    {
    public:
        ConsoleTestsFixture()
        {
            echis::console::ResetGlobalState();
        }
    };

    TEST_CASE_METHOD(ConsoleTestsFixture, "Setting font", "[consoleTests]")
    {
        char dummyBitmap;
        echis::console::FontInfo newFontInfo = {
            .bitmapData = &dummyBitmap,
            .sizeX      = 8,
            .sizeY      = 8
        };
        echis::console::FontInfo mockFontInfo;

        echis::console::Init(newFontInfo, 320, 200, mockFontInfo);

        REQUIRE(newFontInfo.bitmapData == mockFontInfo.bitmapData);
        REQUIRE(newFontInfo.sizeX      == mockFontInfo.sizeX);
        REQUIRE(newFontInfo.sizeY      == mockFontInfo.sizeY);
    }

    TEST_CASE_METHOD(ConsoleTestsFixture, "Put character at location", "[consoleTests]")
    {
        std::unique_ptr<uint8_t[]> bitmap = std::make_unique<uint8_t[]>(8 * 256);
        echis::console::FontInfo fontInfo = {
            .bitmapData = bitmap.get(),
            .sizeX      = 8,
            .sizeY      = 8
        };
        echis::console::Init(fontInfo, 320, 200);

        for (int i = 0; i < 100; ++i)
        {
            mock::echis::framebuffer::DrawBitmapInvocations.clear();

            std::mt19937 rng;
            std::uniform_int_distribution<int> rowUid(0, 40);
            std::uniform_int_distribution<int> colUid(0, 25);
            std::uniform_int_distribution<uint8_t> byteUid(0, 0xff);

            int x = rowUid(rng);
            int y = colUid(rng);

            int foregroundR = byteUid(rng);
            int foregroundG = byteUid(rng);
            int foregroundB = byteUid(rng);
            int backgroundR = byteUid(rng);
            int backgroundG = byteUid(rng);
            int backgroundB = byteUid(rng);

            unsigned char c = byteUid(rng);

            echis::console::PutCharAt<mock::echis::framebuffer>(c, x, y,
                                                              foregroundR << 16 | foregroundG << 8 | foregroundB,
                                                              backgroundR << 16 | backgroundG << 8 | backgroundB);

            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations.size() == 1);

            // Passed in values should be exactly the same
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].bitmap == (bitmap.get() + c * 8));
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].x == x * 8);
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].y == y * 8);
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].sizeX == 8);
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].sizeY == 8);
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].foreground == (foregroundR << 16 | foregroundG << 8 | foregroundB));
            REQUIRE(mock::echis::framebuffer::DrawBitmapInvocations[0].background == (backgroundR << 16 | backgroundG << 8 | backgroundB));
        }
    }

    TEST_CASE_METHOD(ConsoleTestsFixture, "Put character", "[consoleTests]")
    {
        std::unique_ptr<uint8_t[]> bitmap = std::make_unique<uint8_t[]>(8 * 256);
        echis::console::FontInfo fontInfo = {
            .bitmapData = bitmap.get(),
            .sizeX      = 8,
            .sizeY      = 8
        };
        echis::console::Init(fontInfo, 320, 200);

        for (int i = 0; i < 100; ++i)
        {
            mock::echis::console::PutCharAtInvocations.clear();
            mock::echis::console::PutCharMoveCursorInvocations.clear();

            std::mt19937 rng;
            std::uniform_int_distribution<uint8_t> byteUid(0, 0xff);

            int foregroundR = byteUid(rng);
            int foregroundG = byteUid(rng);
            int foregroundB = byteUid(rng);
            int backgroundR = byteUid(rng);
            int backgroundG = byteUid(rng);
            int backgroundB = byteUid(rng);

            unsigned char c = byteUid(rng);

            echis::console::PutChar<mock::echis::console>(c,
                                                          foregroundR << 16 | foregroundG << 8 | foregroundB,
                                               backgroundR << 16 | backgroundG << 8 | backgroundB);

            REQUIRE(mock::echis::console::PutCharAtInvocations.size()         == 1);
            REQUIRE(mock::echis::console::PutCharMoveCursorInvocations.size() == 1);

            // Passed in values should be exactly the same
            REQUIRE(mock::echis::console::PutCharAtInvocations[0].c == c);
            REQUIRE(mock::echis::console::PutCharAtInvocations[0].foreground == (foregroundR << 16 | foregroundG << 8 | foregroundB));
            REQUIRE(mock::echis::console::PutCharAtInvocations[0].background == (backgroundR << 16 | backgroundG << 8 | backgroundB));
        }
    }

    TEST_CASE_METHOD(ConsoleTestsFixture, "Printing string array with size", "[consoleTests]")
    {
        std::unique_ptr<uint8_t[]> bitmap = std::make_unique<uint8_t[]>(8 * 256);
        echis::console::FontInfo fontInfo = {
            .bitmapData = bitmap.get(),
            .sizeX      = 8,
            .sizeY      = 8
        };
        echis::console::Init(fontInfo, 320, 200);

        for (int i = 1; i < 100; ++i)
        {
            mock::echis::console::PutCharInvocations.clear();

            std::mt19937 rng;
            std::uniform_int_distribution<uint8_t> colorUid(0, 0xff);

            constexpr const char characters[] = "0123456789"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz";

            std::uniform_int_distribution<char> charUid(0, sizeof(characters) - 1);

            std::string str;
            str.reserve(i);
            for (int j = 0; j < i; ++j)
            {
                str += characters[charUid(rng)];
            }

            int foregroundR = colorUid(rng);
            int foregroundG = colorUid(rng);
            int foregroundB = colorUid(rng);
            int backgroundR = colorUid(rng);
            int backgroundG = colorUid(rng);
            int backgroundB = colorUid(rng);

            echis::console::PutString<mock::echis::console>(str.data(), i,
                                                            foregroundR << 16 | foregroundG << 8 | foregroundB,
                                                            backgroundR << 16 | backgroundG << 8 | backgroundB);

            REQUIRE(mock::echis::console::PutCharInvocations.size() == i);

            // Passed in values should be exactly the same
            for (int j = 0; j < i; ++j)
            {
                REQUIRE(mock::echis::console::PutCharInvocations[j].c == str[j]);
                REQUIRE(mock::echis::console::PutCharInvocations[j].foreground == (foregroundR << 16 | foregroundG << 8 | foregroundB));
                REQUIRE(mock::echis::console::PutCharInvocations[j].background == (backgroundR << 16 | backgroundG << 8 | backgroundB));
            }
        }
    }

    TEST_CASE_METHOD(ConsoleTestsFixture, "Printing strings", "[consoleTests]")
    {
        std::unique_ptr<uint8_t[]> bitmap = std::make_unique<uint8_t[]>(8 * 256);
        echis::console::FontInfo fontInfo = {
            .bitmapData = bitmap.get(),
            .sizeX      = 8,
            .sizeY      = 8
        };
        echis::console::Init(fontInfo, 320, 200);

        for (int i = 1; i < 100; ++i)
        {
            mock::echis::console::PutCharInvocations.clear();

            std::mt19937 rng;
            std::uniform_int_distribution<uint8_t> colorUid(0, 0xff);

            constexpr const char characters[] = "0123456789"
                                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                                "abcdefghijklmnopqrstuvwxyz";

            std::uniform_int_distribution<int> charUid(0, sizeof(characters) - 2);

            std::string str;
            for (int j = 0; j < i; ++j)
            {
                str += characters[charUid(rng)];
            }

            int foregroundR = colorUid(rng);
            int foregroundG = colorUid(rng);
            int foregroundB = colorUid(rng);
            int backgroundR = colorUid(rng);
            int backgroundG = colorUid(rng);
            int backgroundB = colorUid(rng);

            echis::console::Print<mock::echis::console>(str.data(),
                                                            foregroundR << 16 | foregroundG << 8 | foregroundB,
                                                            backgroundR << 16 | backgroundG << 8 | backgroundB);

            REQUIRE(mock::echis::console::PutCharInvocations.size() == str.size());

            // Passed in values should be exactly the same
            for (int j = 0; j < i; ++j)
            {
                REQUIRE(mock::echis::console::PutCharInvocations[j].c == str[j]);
                REQUIRE(mock::echis::console::PutCharInvocations[j].foreground == (foregroundR << 16 | foregroundG << 8 | foregroundB));
                REQUIRE(mock::echis::console::PutCharInvocations[j].background == (backgroundR << 16 | backgroundG << 8 | backgroundB));
            }
        }
    }
}