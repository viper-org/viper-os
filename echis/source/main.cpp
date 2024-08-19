#include <main.h>

#include <mm/heap.h>
#include <mm/vm/alloc.h>

#include <driver/debugcon.h>
#include <driver/ldr/loader.h>

#include <fs/testfs.h>
#include <fs/devfs.h>

#include <sched/sched.h>

#include <sched/sched.h>

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
    void TestThreadThing()
    {
        driver::debugcon::Write("hello from thread\n");
        while(1) sched::Yield();
    }
    
    void KernelMain()
    {
        atheris::vm::Init();
        mm::Init();

        vm::alloc::Init();
        mm::MarkMemUsed();
        auto pages = vm::alloc::GetKernelPages(4, atheris::vm::flags::present | atheris::vm::flags::write);
        driver::debugcon::WriteFormatted("VMM Allocated 4 pages at %p\n", pages); 

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

        sched::Init();
        auto addproc = [](){
            sched::Process proc1;
            proc1.getMainThread()->getExecStart() = reinterpret_cast<std::uint64_t>(TestThreadThing);
            sched::AddProcess(std::move(proc1));
        };
        addproc();
        addproc();
        sched::Start();
    }
}