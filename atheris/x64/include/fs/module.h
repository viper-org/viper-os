#ifndef VIPEROS_ATHERIS_X64_FS_MODULE_H
#define VIPEROS_ATHERIS_X64_FS_MODULE_H 1

#include <stdint.h>

namespace x64
{
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
}

#endif // VIPEROS_ATHERIS_X64_FS_MODULE_H