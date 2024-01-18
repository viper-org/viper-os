#ifndef VIPEROS_ECHIS_FS_DEVFS_H
#define VIPEROS_ECHIS_FS_DEVFS_H 1

#include <fs/vfs.h>

namespace echis
{
    namespace fs
    {
        namespace devfs
        {
            using ReadFn  = int(*)(void*, size_t*, size_t);
            using WriteFn = int(*)(const void*, size_t, size_t);
            using IoctlFn = int(*)(unsigned long, char*);

            class DeviceNode : public vfs::Node
            {
            public:
                DeviceNode() = default;
                DeviceNode(const vpr::string& path, ReadFn read, WriteFn write, IoctlFn ioctl);
                DeviceNode(DeviceNode&& other);
                DeviceNode& operator=(const DeviceNode& other);
                DeviceNode& operator=(DeviceNode&& other);

                int read(void* buffer, size_t* count, size_t seek) override;
                int write(const void* buffer, size_t count, size_t seek) override;
                void close() override;
                int ioctl(unsigned long request, char* arg) override;

            private:
                ReadFn  mRead;
                WriteFn mWrite;
                IoctlFn mIoctl;
            };

            class DeviceFilesystem : public vfs::Filesystem
            {
            public:
                DeviceFilesystem();

                vfs::Node* lookup(const vpr::string& path) override;
                void       create(const vpr::string& path) override;
            };

            void Init();
            void AddDeviceFile(const vpr::string& path, ReadFn read, WriteFn write, IoctlFn ioctl);
        }
    }
}

#endif // VIPEROS_ECHIS_FS_DEVFS_H