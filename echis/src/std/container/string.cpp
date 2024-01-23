#include <std/container/string.h>
#include <std/utility.h>

#include <string.h>

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
        : mSize(other.mSize)
    {
        mBuffer = new char[mSize + 1];
        memcpy(mBuffer, other.mBuffer, mSize + 1);
    }

    string::string(string&& other)
        : mBuffer(vpr::move(other.mBuffer))
        , mSize(vpr::move(other.mSize))
    {
        other.mBuffer = nullptr;
        other.mSize = 0;
    }

    string::~string()
    {
        delete[] mBuffer;
        mBuffer = nullptr;
        mSize = 0;
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

    string& string::operator=(const string& other)
    {
        if (mBuffer)
        {
            delete[] mBuffer;
        }

        mSize = other.mSize;
        mBuffer = new char[mSize + 1];
        memcpy(mBuffer, other.mBuffer, mSize + 1);

        return *this;
    }
}