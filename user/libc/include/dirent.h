#ifndef VIPEROS_LIBC_DIRENT
#define VIPEROS_LIBC_DIRENT

typedef struct DIR_impl DIR;

struct dirent
{
    unsigned char d_type;
    char d_name[256];
};

enum dirent_type
{
    DT_DIR,
    DT_REG
};

DIR *opendir(const char *name);
int readdir(DIR *dirp, struct dirent *out);

#endif // VIPEROS_LIBC_DIRENT
