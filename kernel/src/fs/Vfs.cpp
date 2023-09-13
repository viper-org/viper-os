#include <fs/Vfs.h>

#include <container/lazy.h>
#include <container/list.h>

#include <string.h>

namespace fs
{
    namespace vfs
    {
        vpr::lazy<vpr::list<Filesystem*> > filesystems;

        Node::Node(const vpr::string& path, Filesystem* filesystem)
            : mPath(path)
            , mFilesystem(filesystem)
        {
        }

        const vpr::string& Node::getPath() const
        {
            return mPath;
        }

        Filesystem::Filesystem(const vpr::string& name)
            : mName(name)
        {
        }

        const vpr::string& Filesystem::getName() const
        {
            return mName;
        }

        Node* Filesystem::getRoot()
        {
            return mRoot;
        }

        void Filesystem::Register(Filesystem* fs)
        {
            filesystems->push_front(fs);
        }

        Node* lookup(const vpr::string& path)
        {
            int index = 0;
            while(path[index] != ':')
            {
                index++;
            }

            char* filesystemName = new char[index + 1];
            memcpy(filesystemName, path.c_str(), index);
            filesystemName[index + 1] = 0;

            vpr::string pathName = path.c_str() + index + 1;

            for(auto filesystem : *filesystems)
            {
                if (filesystem->getName() == filesystemName)
                {
                    delete[] filesystemName;
                    return filesystem->lookup(pathName);
                }
            }
            delete[] filesystemName;
            return nullptr;
        }

        void create(const vpr::string& path)
        {
            int index = 0;
            while(path[index] != ':')
            {
                index++;
            }

            char* filesystemName = new char[index + 1];
            memcpy(filesystemName, path.c_str(), index);
            filesystemName[index + 1] = 0;

            vpr::string pathName = path.c_str() + index + 1;

            for(auto filesystem : *filesystems)
            {
                if (filesystem->getName() == filesystemName)
                {
                    filesystem->create(pathName);
                }
            }
            delete[] filesystemName;
        }
    }
}