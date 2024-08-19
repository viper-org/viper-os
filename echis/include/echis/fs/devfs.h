#ifndef VIPEROS_ECHIS_FS_DEVFS_H
#define VIPEROS_ECHIS_FS_DEVFS_H 1

#include <fs/vfs.h>

#include <driver/ldr/loader.h>

#include <string>
#include <vector>

namespace echis
{
    namespace fs
    {
        namespace dev
        {
            class DevFilesystem : public VirtualFilesystem
            {
            public:
                DevFilesystem();

                virtual int root(std::shared_ptr<VirtualNode>& vnode) override;
                virtual int mount(const std::string& path) override;

                static void AddDevice(driver::ldr::DriverHeader* driver);
                static void Init();
                static VirtualFilesystem* Get();

            private:
                void addDevice(driver::ldr::DriverHeader* driver);

                std::shared_ptr<VirtualNode> mRootNode;
            };

            class DevNode : public VirtualNode
            {
            friend class DevFilesystem;
            friend class DevfsRoot;
            public:
                DevNode(driver::ldr::DriverHeader* driver);

                virtual VNodeOpError read(void* buffer, std::size_t count) override;
                virtual VNodeOpError write(const void* buffer, std::size_t count) override;
                virtual VNodeOpError lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError create(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;

            private:
                driver::ldr::DriverHeader* mDriver;
            };

            class DevfsRoot : public VirtualNode
            {
            public:
                DevfsRoot();

                virtual VNodeOpError read(void* buffer, std::size_t count) override;
                virtual VNodeOpError write(const void* buffer, std::size_t count) override;
                virtual VNodeOpError lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError create(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;

                void addDevice(driver::ldr::DriverHeader* driver);

            private:
                std::vector<std::shared_ptr<DevNode> > mDrivers;
            };
        }
    }
}

#endif // VIPEROS_ECHIS_FS_DEVFS_H