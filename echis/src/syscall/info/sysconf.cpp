#include <syscall/info/sysconf.h>

#include <mm/pmm.h>

namespace echis
{
    namespace syscall
    {
        long sysconf(int name)
        {
            switch(name)
            {
                case SysconfNames::_SC_PAGE_SIZE:
                    return pmm::GetPageSize();
                default:
                    return -1; // TODO: Set errno
            }
        }
    }
}