#ifndef VIPEROS_ECHIS_SYSCALL_FILE_MOVFD_H
#define VIPEROS_ECHIS_SYSCALL_FILE_MOVFD_H 1

namespace echis
{
    namespace syscall
    {
        int movfd(int oldfd, int newfd);
    }
}

#endif // VIPEROS_ECHIS_SYSCALL_FILE_MOVFD_H