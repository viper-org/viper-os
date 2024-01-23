#include <fs/devfs.h>

#include <std/container/vector.h>
#include <std/container/late_init.h>

#include <string.h>

namespace echis
{
    namespace fs
    {
        namespace devfs
        {
            vpr::late_init<DeviceFilesystem> fs;
            vpr::late_init<vpr::vector<DeviceNode>> nodes;

            DeviceNode::DeviceNode(const vpr::string& path, ReadFn read, WriteFn write, IoctlFn ioctl)
                : vfs::Node(path, &fs)
                , mRead(read)
                , mWrite(write)
                , mIoctl(ioctl)
            {
            }

            DeviceNode::DeviceNode(DeviceNode&& other)
                : vfs::Node(vpr::move(other.mPath), other.mParent)
                , mRead(other.mRead)
                , mWrite(other.mWrite)
                , mIoctl(other.mIoctl)
            {
                other.mRead  = nullptr;
                other.mWrite = nullptr;
                other.mIoctl = nullptr;
            }

            DeviceNode& DeviceNode::operator=(const DeviceNode& other)
            {
                mRead  = other.mRead;
                mWrite = other.mWrite;
                mIoctl = other.mIoctl;
                mPath  = other.mPath;
                return *this;
            }

            DeviceNode& DeviceNode::operator=(DeviceNode&& other)
            {
                mRead  = other.mRead;
                mWrite = other.mWrite;
                mIoctl = other.mIoctl;
                
                mPath = vpr::move(other.mPath);

                other.mRead = nullptr;
                other.mWrite = nullptr;
                other.mIoctl = nullptr;

                return *this;
            }

            int DeviceNode::read(void* buffer, size_t* count, size_t seek)
            {
                return mRead(buffer, count, seek);
            }

            int DeviceNode::write(const void* buffer, size_t count, size_t seek)
            {
                return mWrite(buffer, count, seek);
            }

            void DeviceNode::close()
            { // Nothing to do here
            }

            int DeviceNode::ioctl(unsigned long request, char* arg)
            {
                return mIoctl(request, arg);
            }

            size_t DeviceNode::size()
            {
                return 0;
            }

            DeviceFilesystem::DeviceFilesystem()
                : vfs::Filesystem("dev")
            {
            }

            vfs::Node* DeviceFilesystem::lookup(const vpr::string& path)
            {
                for (auto& node : *nodes)
                {
                    if (node.getPath() == path)
                    {
                        return &node;
                    }
                }
                return nullptr;
            }

            void DeviceFilesystem::create(const vpr::string&)
            { // Not allowed
            }

            void Init()
            {
                fs = DeviceFilesystem();
                vfs::Filesystem::Register(&fs);
            }

            void AddDeviceFile(const vpr::string& path, ReadFn read, WriteFn write, IoctlFn ioctl)
            {
                nodes->push_back(DeviceNode(path, read, write, ioctl));
            }
        }
    }
}