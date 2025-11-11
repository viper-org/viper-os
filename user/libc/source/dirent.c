#include <dirent.h>

#include <unistd.h>
#include <stdlib.h>

struct DIR_impl
{
    int fd;
};

DIR *opendir(const char *name)
{
    int fd = open(name, O_DIRECTORY);
    if (fd < 0) return NULL;
    DIR *dir = malloc(sizeof (struct DIR_impl));
    dir->fd = fd;
    return dir;
}

int readdir(DIR *dirp, struct dirent *out)
{
    return read(dirp->fd, out, sizeof (struct dirent));
}
