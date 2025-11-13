#ifndef VIPEROS_LIBC_SYS_UTSNAME
#define VIPEROS_LIBC_SYS_UTSNAME 1

struct utsname
{
    char sysname[32];
    char nodename[64];
    char release[32];
    char version[128];
    char machine[32];
    char domainname[1];
};

int uname(struct utsname *buf);

#endif // VIPEROS_LIBC_SYS_UTSNAME
