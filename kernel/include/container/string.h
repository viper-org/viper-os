#ifndef VIPER_OS_CONTAINER_STRING_H
#define VIPER_OS_CONTAINER_STRING_H 1

#include <memory.h>

namespace vpr
{
    class string
    {
    public:
        string();
        string(const char* buffer);
        string(const string& other);
        string(string&& other);
        ~string();

        const char* c_str() const;
        size_t size() const;

        bool operator==(const string& other) const;
        char operator[](int index) const;

    private:
        char* mBuffer;
        size_t mSize;
    };
}

#endif