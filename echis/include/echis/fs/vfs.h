#ifndef VIPEROS_ECHIS_FS_VFS_H
#define VIPEROS_ECHIS_FS_VFS_H 1

#include <cstddef>
#include <memory>
#include <string>

namespace echis
{
    namespace fs
    {
        enum class VNodeType
        {
            File,
            Directory
        };

        enum class VNodeOpError
        {
            Success=0,
            IsDirectory,
            IsFile,
            NotFound,
        };

        struct VirtualNode;

        struct VirtualFilesystem
        {
            VirtualFilesystem();

            virtual ~VirtualFilesystem();
            virtual int root(std::shared_ptr<VirtualNode>& vnode) = 0;

            static void AddMountedFS(VirtualFilesystem* fs);

            VirtualFilesystem* next; // next filesystem
            VirtualNode* nodeCovered; // vnode where this FS is mounted
        };

        struct VirtualNode
        {
            VirtualNode(VirtualFilesystem* fs, VNodeType type);

            virtual ~VirtualNode();
            virtual VNodeOpError read(void* buffer, std::size_t count) = 0;
            virtual VNodeOpError write(const void* buffer, std::size_t count) = 0;
            virtual VNodeOpError lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode) = 0;
            virtual VNodeOpError create(const std::string& name, std::shared_ptr<VirtualNode>& vnode) = 0; // TODO: Add openmode, exclusivity and attributes
            virtual VNodeOpError mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode) = 0; // TODO: Add attributes

            VirtualFilesystem* vfs;
            VirtualFilesystem* mountedVFS;
            VNodeType type;
        };

        std::shared_ptr<VirtualNode> LookupPathName(const std::string& path);
    }
}

#endif // VIPEROS_ECHIS_FS_VFS_H