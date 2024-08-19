#include <fs/vfs.h>

#include <driver/debugcon.h>

namespace echis
{
    namespace fs
    {
        VirtualFilesystem* filesystems;

        VirtualFilesystem::VirtualFilesystem()
        {
        }

        VirtualFilesystem::~VirtualFilesystem()
        {
        }

        void VirtualFilesystem::AddMountedFS(VirtualFilesystem* fs)
        {
            if (!filesystems)
            {
                filesystems = fs;
            }
            else
            {
                auto current = filesystems;
                while (current->next)
                {
                    current = current->next;
                }
                current->next = fs;
            }
        }

        int VirtualFilesystem::defaultMount(const std::string& path)
        {
            std::shared_ptr<VirtualNode> node = LookupPathName(path);
            if (!node || node->mountedVFS)
            {
                return -1;
            }
            nodeCovered = node.get();
            node->mountedVFS = this;
            return 0;
        }


        VirtualNode::VirtualNode(VirtualFilesystem* fs, VNodeType type)
            : vfs(fs)
            , type(type)
        {
        }

        VirtualNode::~VirtualNode()
        {
        }


        std::shared_ptr<VirtualNode> LookupPathName(const std::string& path)
        {
            std::shared_ptr<VirtualNode> root;
            filesystems->root(root);

            std::shared_ptr<VirtualNode> currentNode = root;

            // absolute paths only for now
            if (path[0] != '/') return nullptr;

            char buffer[129];
            std::size_t index = 1;
            int bufferIndex = 0;
            while (index < path.length())
            {
                while (path[index] != '/' && index < path.length())
                {
                    buffer[bufferIndex] = path[index];
                    ++index;
                    ++bufferIndex;
                }
                buffer[bufferIndex] = '\0';

                // we have a path component now
                if (auto err = currentNode->lookup(buffer, currentNode); err != VNodeOpError::Success)
                {
                    driver::debugcon::WriteFormatted("Path lookup component %s returned %d\n", buffer, err);
                    return nullptr;
                }
                if (currentNode->mountedVFS)
                {
                    currentNode->mountedVFS->root(currentNode);
                }

                bufferIndex = 0;
                ++index;
            }

            return currentNode;
        }
    }
}