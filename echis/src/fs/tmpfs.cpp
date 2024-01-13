#include <fs/tmpfs.h>

#include <std/container/vector.h>
#include <std/container/late_init.h>

#include <string.h>

namespace echis
{
    namespace fs
    {
        namespace tmpfs
        {
            vpr::late_init<Filesystem> fs;
            vpr::late_init<vpr::vector<Node>> nodes;

            Node::Node(const vpr::string& path)
                : vfs::Node(path, &fs)
                , mData(nullptr)
                , mSize(0)
                , mCapacity(0)
            {
            }

            Node::~Node()
            {
                if (mData)
                {
                    delete mData;
                }
            }

            int Node::read(void* buffer, size_t* count)
            {
                if (*count >= mSize)
                {
                    *count = mSize;
                }

                memcpy(buffer, mData, *count);
                return 0;
            }

            int Node::write(const void* buffer, size_t count)
            {
                if (mCapacity < count)
                {
                    delete[] mData;
                    mData = new char[count];
                    mCapacity = count;
                }
                memcpy(mData, buffer, count);
                mSize = count;
                return 0;
            }

            Filesystem::Filesystem()
                : vfs::Filesystem("tmp")
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
                nodes->push_back(Node(path));
            }


            void Init()
            {
                fs = Filesystem();
                vfs::Filesystem::Register(&fs);
            }
        }
    }
}