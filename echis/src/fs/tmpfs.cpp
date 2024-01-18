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

            Node::Node(Node&& other)
                : mData(other.mData)
                , mSize(other.mSize)
                , mCapacity(other.mCapacity)
            {
                other.mData = nullptr;
                other.mSize = 0;
                other.mCapacity = 0;
            }

            Node& Node::operator=(const Node& other)
            {
                if (mCapacity >= other.mCapacity)
                {
                    memcpy(mData, other.mData, other.mSize);
                    mSize = other.mSize;
                }
                else
                {
                    delete[] mData;
                    mData = new char[other.mCapacity];
                    memcpy(mData, other.mData, other.mSize);
                    mSize = other.mSize;
                    mCapacity = other.mCapacity;
                }
                mPath = other.mPath;
                return *this;
            }

            Node& Node::operator=(Node&& other)
            {
                if (mData)
                {
                    delete mData;
                }
                mData = other.mData;
                mSize = other.mSize;
                mCapacity = other.mCapacity;
                mPath = std::move(other.mPath);

                other.mData = nullptr;
                other.mSize = 0;
                other.mCapacity = 0;

                return *this;
            }

            Node::~Node()
            {
                if (mData)
                {
                    delete mData;
                }
            }

            int Node::read(void* buffer, size_t* count, size_t seek)
            {
                if (*count >= mSize)
                {
                    *count = mSize;
                }

                memcpy(buffer, mData + seek, *count);
                return 0;
            }

            int Node::write(const void* buffer, size_t count, size_t seek)
            {
                if (mCapacity - seek < count)
                {
                    delete[] mData;
                    mData = new char[count + seek];
                    mCapacity = count + seek;
                }
                memcpy(mData + seek, buffer, count);
                mSize = count;
                return 0;
            }

            void Node::close()
            { // Nothing to do here
            }

            int Node::ioctl(unsigned long, char*)
            { // Only device files can be ioctl'd
                return 0;
            }

            size_t Node::size()
            {
                return mSize;
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