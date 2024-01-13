#include <fs/vfs.h>

#include <std/container/vector.h>

#include <string.h>

namespace echis
{
    namespace fs
    {
        namespace vfs
        {
            vpr::vector<Filesystem*> filesystems;

            Node::Node(const vpr::string& path, Filesystem* parent)
                : mPath(path)
                , mParent(parent)
            {
            }

            Filesystem* Node::getParent() const
            {
                return mParent;
            }

            const vpr::string& Node::getPath() const
            {
                return mPath;
            }

            Filesystem::Filesystem(const vpr::string& ident)
                : mIdent(ident)
            {
            }

            const vpr::string& Filesystem::getIdent() const
            {
                return mIdent;
            }

            void Filesystem::Register(Filesystem* fs)
            {
                filesystems.push_back(fs);
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

                for(auto filesystem : filesystems)
                {
                    if (filesystem->getIdent() == filesystemName)
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

                for(auto filesystem : filesystems)
                {
                    if (filesystem->getIdent() == filesystemName)
                    {
                        filesystem->create(pathName);
                    }
                }
                delete[] filesystemName;
            }
        }
    }
}