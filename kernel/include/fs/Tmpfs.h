#ifndef VIPER_OS_FS_TMPFS_H
#define VIPER_OS_FS_TMPFS_H 1

#include <fs/Vfs.h>

namespace fs
{
    namespace tmpfs
    {
        void Init();

        class Node : public vfs::Node
        {
        friend class Filesystem;
        public:
            Node(const vpr::string& path);
            ~Node();

            int read(void* buffer, size_t* count) override;
            int write(const void* buffer, size_t count) override;

        private:
            char* mData;
            size_t mSize;
            size_t mCapacity;
        };

        class Filesystem : public vfs::Filesystem
        {
        public:
            Filesystem();

            vfs::Node* lookup(const vpr::string& path) override;
            void create(const vpr::string& path) override;
        };
    }
}

#endif