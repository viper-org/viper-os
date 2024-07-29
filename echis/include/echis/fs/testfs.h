#ifndef VIPEROS_ECHIS_FS_TESTFS_H
#define VIPEROS_ECHIS_FS_TESTFS_H 1

#include <fs/vfs.h>

#include <string>
#include <vector>

namespace echis
{
    namespace fs
    {
        namespace test
        {
            class TestVFilesystem : public VirtualFilesystem
            {
            public:
                TestVFilesystem();

                virtual int root(std::shared_ptr<VirtualNode>& vnode) override;

                static void Init();

            private:
                std::shared_ptr<VirtualNode> mRootNode;
            };

            class TestNodeFile : public VirtualNode
            {
            friend class TestVFilesystem;
            friend class TestNodeDir;
            public:
                TestNodeFile(std::string name);

                virtual VNodeOpError read(void* buffer, std::size_t count) override;
                virtual VNodeOpError write(const void* buffer, std::size_t count) override;
                virtual VNodeOpError lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError create(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;

            private:
                std::string mName;
                char* mData;
                std::size_t mSize;
            };

            class TestNodeDir : public VirtualNode
            {
            friend class TestVFilesystem;
            public:
                TestNodeDir(std::string name);

                virtual VNodeOpError read(void* buffer, std::size_t count) override;
                virtual VNodeOpError write(const void* buffer, std::size_t count) override;
                virtual VNodeOpError lookup(const std::string& component, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError create(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;
                virtual VNodeOpError mkdir(const std::string& name, std::shared_ptr<VirtualNode>& vnode) override;

            private:
                std::string mName;
                std::vector<std::shared_ptr<VirtualNode> > mContained;
            };
        }
    }
}

#endif // VIPEROS_ECHIS_FS_TESTFS_H