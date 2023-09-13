#include <fs/DevFs.h>
#include <fs/Vfs.h>

#include <container/lazy.h>
#include <container/list.h>

namespace fs
{
    namespace devfs
    {
        vpr::lazy<Filesystem> fs;
        vpr::lazy<vpr::list<Node>> nodes;

        Node::Node(const vpr::string& path, Node::ReadFn read, Node::WriteFn write)
            : vfs::Node(path, &*fs), mRead(read), mWrite(write)
        {
        }

        int Node::read(void* buffer, size_t* count)
        {
            mRead(buffer, count);
            return 0;
        }

        int Node::write(const void* buffer, size_t count)
        {
            mWrite(buffer, count);
            return 0;
        }


        Filesystem::Filesystem()
            : vfs::Filesystem("dev")
        {
        }

        void Filesystem::RegisterDeviceFile(const vpr::string& path, Node::ReadFn read, Node::WriteFn write)
        {
            nodes->push_front(Node(path, read, write));
        }

        vfs::Node* Filesystem::lookup(const vpr::string& path)
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

        void Filesystem::create(const vpr::string&)
        {
            // TODO: Error(invalid)
        }


        void Init()
        {
            fs = Filesystem();
            vfs::Filesystem::Register(&*fs);
        }
    }
}