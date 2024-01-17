#include <syscall/file/open.h>

#include <fs/vfs.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int open(const char* path, int flags)
        {
            fs::vfs::Node* node = fs::vfs::lookup(path);
            if (node)
            {
                return sched::Current()->getParent()->addOpenFileDescription(node, static_cast<sched::OpenMode::OpenMode>(flags));
            }
            return -1;
        }
    }
}