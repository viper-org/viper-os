#include <fs/devfs.h>

#include <lazy_init.h>

namespace echis
{
    namespace fs
    {
        namespace dev
        {
            static lazy_init<DevFilesystem> devfs;

            DevFilesystem::DevFilesystem()
            {
            }

            int DevFilesystem::root(std::shared_ptr<VirtualNode>& vnode)
            {
                vnode = mRootNode;
                return 0;
            }

            int DevFilesystem::mount(const std::string& path)
            {
                return defaultMount(path);
            }

            void DevFilesystem::addDevice(driver::ldr::DriverHeader* driver)
            {
                auto root = static_cast<DevfsRoot*>(mRootNode.get());
                root->addDevice(driver);
            }

            void DevFilesystem::AddDevice(driver::ldr::DriverHeader* driver)
            {
                devfs->addDevice(driver);
            }

            void DevFilesystem::Init()
            {
                devfs = DevFilesystem();
                devfs->mRootNode = std::make_shared<DevfsRoot>();
            }

            VirtualFilesystem* DevFilesystem::Get()
            {
                return &devfs;
            }


            DevNode::DevNode(driver::ldr::DriverHeader* driver)
                : VirtualNode(&devfs, VNodeType::File)
                , mDriver(std::move(driver))
            {
            }

            VNodeOpError DevNode::read(void* buffer, std::size_t count)
            {
                mDriver->read(buffer, count);
                return VNodeOpError::Success; // TODO: Return ssize_t here
            }

            VNodeOpError DevNode::write(const void* buffer, std::size_t count)
            {
                mDriver->write(buffer, count);
                return VNodeOpError::Success; // TODO: Return ssize_t here
            }

            VNodeOpError DevNode::lookup(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }

            VNodeOpError DevNode::create(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }

            VNodeOpError DevNode::mkdir(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }


            DevfsRoot::DevfsRoot()
                : VirtualNode(&devfs, VNodeType::Directory)
            {
            }

            VNodeOpError DevfsRoot::read(void*, std::size_t)
            {
                return VNodeOpError::IsDirectory;
            }

            VNodeOpError DevfsRoot::write(const void*, std::size_t)
            {
                return VNodeOpError::IsDirectory;
            }

            VNodeOpError DevfsRoot::lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode)
            {
                for (const auto& node : mDrivers)
                {
                    if (node->mDriver->name == component)
                    {
                        vnode = node;
                        return VNodeOpError::Success;
                    }
                }
                return VNodeOpError::NotFound;
            }

            VNodeOpError DevfsRoot::create(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsDirectory; // TODO: Add disallowed error
            }

            VNodeOpError DevfsRoot::mkdir(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsDirectory; // TODO: Add disallowed error or implement this
            }

            void DevfsRoot::addDevice(driver::ldr::DriverHeader* driver)
            {
                mDrivers.push_back(std::make_shared<DevNode>(driver));
            }
        }
    }
}