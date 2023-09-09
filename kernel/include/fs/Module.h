#ifndef VIPER_OS_FS_MODULE_H
#define VIPER_OS_FS_MODULE_H 1

#include <cstdint>

namespace fs
{
    class Module
    {
    public:
        Module() = default;

        void* getAddress() const;
        char* getPath() const;
        uint64_t getSize() const;

        static Module Get(const char* path);
    private:
        Module(void* address, char* path, uint64_t size);

        void* mAddress;
        char* mPath;
        uint64_t mSize;
    };
}

#endif // VIPER_OS_FS_MODULE_H