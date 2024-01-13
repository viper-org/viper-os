#include <std/container/string.h>

#include <string.h>

#include <utility>

namespace vpr
{
    string::string()
        : mBuffer(new char[1])
        , mSize(0)
    {
        mBuffer[0] = '\0';
    }

    string::string(const char* buffer)
    {
        if (buffer == nullptr)
        {
            mBuffer = new char[1];
            mSize = 0;
            mBuffer[0] = '\0';
            return;
        }
        size_t length = strlen(buffer);
        mBuffer = new char[length + 1];

        memcpy(mBuffer, buffer, length);
        mBuffer[length] = '\0';

        mSize = length;
    }

    string::string(const string& other)
    {
        mBuffer = new char[other.mSize + 1];

        memcpy(mBuffer, other.mBuffer, other.mSize);
        mBuffer[other.mSize] = '\0';

        mSize = other.mSize;
    }

    string::string(string&& other)
        : mBuffer(std::move(other.mBuffer))
        , mSize(std::move(other.mSize))
    {
    }

    string::~string()
    {
        delete[] mBuffer;
    }

    
    const char* string::c_str() const
    {
        return mBuffer;
    }

    size_t string::size() const
    {
        return mSize;
    }


    bool string::operator==(const string& other) const
    {
        return !strcmp(mBuffer, other.mBuffer);
    }

    char string::operator[](int index) const
    {
        return mBuffer[index];
    }
}