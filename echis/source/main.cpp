#include <main.h>

#include <mm/heap.h>
#include <mm/vm/alloc.h>

#include <driver/debugcon.h>
#include <driver/ldr/loader.h>

#include <fs/testfs.h>
#include <fs/devfs.h>

#include <sched/sched.h>
#include <sched/lock/mutex.h>

#include <atheris/driver/timer.h>

#include <atheris/mm/vm.h>

#include <limine.h>

#include <lazy_init.h>
 
static volatile limine_module_request ModuleRequest = {
    .id = LIMINE_MODULE_REQUEST,
    .revision = 1,
    .response = nullptr
};

namespace echis
{
    lazy_init<sched::lock::Mutex> m;

    void TestThreadThing1()
    {
        while(1)
        {
            m->lock();
            driver::debugcon::Write("Proc1\n");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            m->unlock();
        }
    }
    void TestThreadThing2()
    {
        while(1)
        {
            m->lock();
            driver::debugcon::Write("Proc2\n");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            asm("pause");
            m->unlock();
        }
    }
    
    void KernelMain()
    {
        atheris::vm::Init();
        mm::Init();

        vm::alloc::Init();
        mm::MarkMemUsed();
        auto pages = vm::alloc::GetKernelPages(4, atheris::vm::flags::present | atheris::vm::flags::write);
        *(char*)pages = 2;
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
            if (!std::strcmp(ModuleRequest.response->modules[i]->path, "/sample.vdrv"))
            {
                auto hdr = driver::ldr::LoadDriver(ModuleRequest.response->modules[i]->address);
                hdr.init();
                fs::dev::DevFilesystem::AddDevice(hdr.header);
            }
        }

        node = fs::LookupPathName("/dev/sample");
        node->write(nullptr, 0);

        atheris::timer::Init(10e6); // 10ms
        
        m.default_init();
        sched::Init();
        auto addproc = [](void(*fn)()){
            sched::Process proc1;
            proc1.getMainThread()->getExecStart() = reinterpret_cast<std::uint64_t>(fn);
            sched::AddProcess(std::move(proc1));
        };
        addproc(TestThreadThing1);
        addproc(TestThreadThing2);
        sched::Start();
    }
}