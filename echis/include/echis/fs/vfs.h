#ifndef VIPEROS_ECHIS_FS_VFS_H
#define VIPEROS_ECHIS_FS_VFS_H 1

#include <std/container/string.h>

namespace echis
{
    namespace fs
    {
        namespace vfs
        {
            class Filesystem;

            class Node
            {
            public:
                Node() = default;
                Node(const vpr::string& path, Filesystem* parent);

                virtual int  read(void* buffer, size_t* count) = 0;
                virtual int  write(const void* buffer, size_t count) = 0;
                virtual void close() = 0;

                Filesystem* getParent() const;
                const vpr::string& getPath() const;

            protected:
                vpr::string mPath;
                Filesystem* mParent;
            };

            class Filesystem
            {
            public:
                Filesystem(const vpr::string& ident);

                virtual Node* lookup(const vpr::string& path) = 0;
                virtual void  create(const vpr::string& path) = 0;

                const vpr::string& getIdent() const;

                static void Register(Filesystem* fs);

            protected:
                vpr::string mIdent;
            };

            // Generic functions
            Node* lookup(const vpr::string& path);
            void  create(const vpr::string& path);
        }
    }
}

#endif // VIPEROS_ECHIS_FS_VFS_H