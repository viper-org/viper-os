#ifndef VIPEROS_ECHIS_STD_CONTAINER_STRING_H
#define VIPEROS_ECHIS_STD_CONTAINER_STRING_H 1

#include <stddef.h>

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
        string& operator=(const string& other);

    private:
        char* mBuffer;
        size_t mSize;
    };
}

#endif