#ifndef VIPER_OS_FS_VFS_H
#define VIPER_OS_FS_VFS_H 1

#include <container/string.h>

#include <stddef.h>

namespace fs
{
    namespace vfs
    {
        class Filesystem;

        class Node
        {
        public:
            Node(const vpr::string& path, Filesystem* filesystem);

            virtual int read(void* buffer, size_t* count) = 0;
            virtual int write(const void* buffer, size_t count) = 0;

            const vpr::string& getPath() const;

        protected:
            vpr::string mPath;
            Filesystem* mFilesystem;
        };

        class Filesystem
        {
        public:
            Filesystem(const vpr::string& name);

            Node* getRoot();

            virtual Node* lookup(const vpr::string& path) = 0;
            virtual void  create(const vpr::string& path) = 0;

            const vpr::string& getName() const;

            static void Register(Filesystem* fs);

        protected:
            Node* mRoot;
            vpr::string mName;
        };

        // Filesystem generic functions 
        Node* lookup(const vpr::string& path);
        void create(const vpr::string& path);
    }
}

#endif