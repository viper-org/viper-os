#include <main.h>

#include <mm/heap.h>

#include <driver/debugcon.h>
#include <driver/ldr/loader.h>

#include <fs/testfs.h>
#include <fs/devfs.h>

#include <atheris/mm/vm.h>

#include <limine.h>

extern "C" void dosomething()
{
    echis::driver::debugcon::Write("kernel function getter working");
}

extern "C" void KeDebugLog(const char* msg)
{
    echis::driver::debugcon::Write(msg);
}
 
static volatile limine_module_request ModuleRequest = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = nullptr
};

namespace echis
{
    void KernelMain()
    {
        atheris::vm::Init();
        mm::Init();

        fs::test::TestVFilesystem::Init();
        fs::dev::DevFilesystem::Init();
        auto node = fs::LookupPathName("/");
        std::shared_ptr<fs::VirtualNode> dir;
        node->mkdir("dev", dir);

        fs::dev::DevFilesystem::Get()->mount("/dev");

        driver::ldr::Init();
        for (auto i = 0; i < ModuleRequest.response->module_count; ++i)
        {
            if (!std::strcmp(ModuleRequest.response->modules[i]->path, "/sample.elf"))
            {
                auto hdr = driver::ldr::LoadDriver(ModuleRequest.response->modules[i]->address);
                hdr.init();
                fs::dev::DevFilesystem::AddDevice(hdr.header);
            }
        }

        node = fs::LookupPathName("/dev/sample");
        node->write(nullptr, 0);
    }
}