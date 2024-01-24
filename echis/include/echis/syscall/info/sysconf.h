#ifndef VIPEROS_ECHIS_SYSCALL_INFO_SYSCONF_H
#define VIPEROS_ECHIS_SYSCALL_INFO_SYSCONF_H 1

#include <stddef.h>

namespace echis
{
    namespace syscall
    {
        namespace SysconfNames
        {
            enum SysconfNames
            {
                _SC_PAGESIZE  = 0,
                _SC_PAGE_SIZE = 0,
            };
        }

        long sysconf(int name);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_INFO_SYSCONF_H