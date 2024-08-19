#include <fs/testfs.h>

#include <lazy_init.h>

#include <cstring>
#include <utility>

namespace echis
{
    namespace fs
    {
        namespace test
        {
            lazy_init<TestVFilesystem> fs;

            void TestVFilesystem::Init()
            {
                fs = TestVFilesystem();
                fs->mRootNode = std::make_shared<TestNodeDir>("/");
                VirtualFilesystem::AddMountedFS(&fs);
            }

            TestVFilesystem::TestVFilesystem()
            {
            }

            int TestVFilesystem::root(std::shared_ptr<VirtualNode>& vnode)
            {
                vnode = mRootNode;
                return 0;
            }

            int TestVFilesystem::mount(const std::string& path)
            {
                return defaultMount(path);
            }


            TestNodeFile::TestNodeFile(std::string name)
                : VirtualNode(&fs, VNodeType::File)
                , mName(std::move(name))
                , mData(nullptr)
                , mSize(0)
            {
            }

            VNodeOpError TestNodeFile::read(void* buffer, std::size_t count)
            {
                if (count < mSize)
                {
                    count = mSize;
                }

                std::memcpy(buffer, mData, count);

                return VNodeOpError::Success;
            }

            VNodeOpError TestNodeFile::write(const void* buffer, std::size_t count)
            {
                delete[] mData;

                mData = new char[count];
                std::memcpy(mData, buffer, count);

                return VNodeOpError::Success;
            }

            VNodeOpError TestNodeFile::lookup(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }

            VNodeOpError TestNodeFile::create(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }

            VNodeOpError TestNodeFile::mkdir(const std::string&, std::shared_ptr<VirtualNode>&)
            {
                return VNodeOpError::IsFile;
            }


            TestNodeDir::TestNodeDir(std::string name)
                : VirtualNode(&fs, VNodeType::Directory)
                , mName(std::move(name))
            {
            }

            VNodeOpError TestNodeDir::read(void*, std::size_t)
            {
                return VNodeOpError::IsDirectory;
            }

            VNodeOpError TestNodeDir::write(const void*, std::size_t)
            {
                return VNodeOpError::IsDirectory;
            }

            VNodeOpError TestNodeDir::lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode)
            {
                for (auto& contained : mContained)
                {
                    if (contained->type == VNodeType::File)
                    {
                        auto file = static_cast<TestNodeFile*>(contained.get());
                        if (file->mName == component)
                        {
                            vnode = contained;
                            return VNodeOpError::Success;
                        }
                    }
                    else if (contained->type == VNodeType::Directory)
                    {
                        auto dir = static_cast<TestNodeDir*>(contained.get());
                        if (dir->mName == component)
                        {
                            vnode = contained;
                            return VNodeOpError::Success;
                        }
                    }
                }
                return VNodeOpError::NotFound;
            }

            VNodeOpError TestNodeDir::create(const std::string& name, std::shared_ptr<VirtualNode>& vnode)
            {
                mContained.push_back(std::make_shared<TestNodeFile>(name));
                vnode = mContained.back();
                return VNodeOpError::Success;
            }

            VNodeOpError TestNodeDir::mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode)
            {
                mContained.push_back(std::make_shared<TestNodeDir>(name));
                vnode = mContained.back();
                return VNodeOpError::Success;
            }
        }
    }
}