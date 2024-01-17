#ifndef VIPEROS_ECHIS_FS_TMPFS_H
#define VIPEROS_ECHIS_FS_TMPFS_H 1

#include <fs/vfs.h>

namespace echis
{
    namespace fs
    {
        namespace tmpfs
        {
            class Node : public vfs::Node
            {
            public:
                Node() = default;
                Node(const vpr::string& path);
                Node(Node&& other);
                Node& operator=(const Node& other);
                Node& operator=(Node&& other);
                ~Node();

                int read(void* buffer, size_t* count) override;
                int write(const void* buffer, size_t count) override;
                void close() override;
                int ioctl(unsigned long request, char* arg) override;

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
                void       create(const vpr::string& path) override;
            };

            void Init();
        }
    }
}

#endif // VIPEROS_ECHIS_FS_TMPFS_H