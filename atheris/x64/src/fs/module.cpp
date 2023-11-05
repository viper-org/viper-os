#include <fs/module.h>

#include <limine.h>

#include <string.h>

static volatile limine_module_request moduleRequest = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 0,
    .response = nullptr
};

namespace x64
{
    namespace fs
    {
        void* Module::getAddress() const
        {
            return mAddress;
        }

        char* Module::getPath() const
        {
            return mPath;
        }

        uint64_t Module::getSize() const
        {
            return mSize;
        }

        Module Module::Get(const char* path)
        {
            for(uint64_t i = 0; i < moduleRequest.response->module_count; i++)
            {
                if (!strcmp(moduleRequest.response->modules[i]->path, path))
                {
                    return Module(moduleRequest.response->modules[i]->address, moduleRequest.response->modules[i]->path, moduleRequest.response->modules[i]->size);
                }
            }
            return Module(nullptr, nullptr, 0);
        }

        Module::Module(void* address, char* path, uint64_t size)
            : mAddress(address)
            , mPath(path)
            , mSize(size)
        {
        }
    }
}