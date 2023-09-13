#include <fs/Tmpfs.h>

#include <container/lazy.h>
#include <container/list.h>
#include <container/ptr.h>

#include <string.h>

#include <utility>

namespace fs
{
    namespace tmpfs
    {
        vpr::lazy<Filesystem> fs;
        vpr::lazy<vpr::list<Node> > nodes;

        Node::Node(const vpr::string& path)
            : vfs::Node(path, &*fs)
            , mData(nullptr)
            , mSize(0)
            , mCapacity(0)
        {
        }

        Node::~Node()
        {
            delete[] mData;
        }

        int Node::read(void* buffer, size_t* count)
        {
            if (*count > mSize)
            {
                *count = mSize;
            }

            memcpy(buffer, mData, *count);

            return 0;
        }

        int Node::write(const void* buffer, size_t count)
        {
            if (count > mCapacity)
            {
                delete[] mData;
                mData = new char[count];
                memcpy(mData, buffer, count);

                mCapacity = count;
            }
            else
            {
                memcpy(mData, buffer, count);
            }
            mSize = count;
            return 0;
        }


        Filesystem::Filesystem()
            : vfs::Filesystem("tmpfs")
        {
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

        void Filesystem::create(const vpr::string& path)
        {
            vpr::string pathCopy = path;
            nodes->emplace_front(pathCopy);
        }


        void Init()
        {
            fs = Filesystem();
            Filesystem::Register(&*fs);
        }
    }
}