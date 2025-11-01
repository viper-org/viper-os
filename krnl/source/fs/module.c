#include "fs/module.h"

#include <string.h>

static volatile struct limine_module_request modreq = {
    .id = LIMINE_MODULE_REQUEST_ID,
    .revision = 1,
    .response = NULL
};

struct limine_file *get_module(const char *path)
{
    for (uint64_t i = 0; i < modreq.response->module_count; ++i)
    {
        if (!strcmp(path, modreq.response->modules[i]->path))
        {
            return modreq.response->modules[i];
        }
    }
    return NULL;
}
