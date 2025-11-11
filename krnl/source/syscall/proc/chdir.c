#include "syscall/syscalls.h"

#include "sched/proc.h"
#include "sched/sched.h"

#include "mm/kheap.h"

#include "string.h"

int sys_chdir(char *path)
{
    struct process *curr = sched_curr()->owner;

    if (path[0] != '/') // relative path
    {
        size_t size1 = strlen(curr->cwd) + 1;
        size_t size2 = strlen(path) + 1;
        char *new = kheap_alloc(size1 + size2);

        strcpy(new, curr->cwd);
        if (strcmp(curr->cwd, "/")) // there will be a trailing slash if we are in the root dir
        {
            new[size1 - 1] = '/';
            new[size1] = 0;
        }
        strcat(new, path);

        if (path[size2 - 2] == '/')
        {
            new[size1 + size2 - 2] = 0;
        }

        kheap_free(curr->cwd);
        curr->cwd = new;
        return 0;
    }

    kheap_free(curr->cwd);
    curr->cwd = strdup(path);
    size_t len = strlen(path);
    if (path[len-1] == '/' && len != 1) // len != 1 is checking if we are in root dir, see above
    {
        curr->cwd[len-1] = 0;
    }
    return 0;
}
