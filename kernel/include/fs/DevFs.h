#ifndef VIPER_OS_FS_DEVFS_H
#define VIPER_OS_FS_DEVFS_H 1

#include <fs/Vfs.h>

namespace fs
{
    namespace devfs
    {
        void Init();
        class Filesystem;

        class Node : public vfs::Node
        {
        friend class vfs::Filesystem;
        friend class Filesystem;
        public:
            using ReadFn  = void(*)(void* buffer, size_t* count);
            using WriteFn = void(*)(const void* buffer, size_t count);

            int read(void* buffer, size_t* count) override;
            int write(const void* buffer, size_t count) override;

        private:
            Node(const vpr::string& path, Node::ReadFn read, Node::WriteFn write);

            ReadFn mRead;
            WriteFn mWrite;
            size_t mSize;
            size_t mCapacity;
        };

        class Filesystem : public vfs::Filesystem
        {
        public:
            Filesystem();

            static void RegisterDeviceFile(const vpr::string& path, Node::ReadFn read, Node::WriteFn write);

            vfs::Node* lookup(const vpr::string& path) override;
            void create(const vpr::string& path) override;
        };
    }
}

#endif