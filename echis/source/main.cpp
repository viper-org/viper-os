#include <main.h>

#include <mm/heap.h>

#include <driver/debugcon.h>

#include <fs/testfs.h>

#include <atheris/mm/vm.h>

namespace echis
{
    void KernelMain()
    {
        atheris::vm::Init();
        mm::Init();

        fs::test::TestVFilesystem::Init();
        auto node = fs::LookupPathName("/");
        std::shared_ptr<fs::VirtualNode> dir;
        node->mkdir("foo", dir);

        std::shared_ptr<fs::VirtualNode> file;
        dir->create("test", file);

        file = fs::LookupPathName("/foo/test");
        auto data = "hello world";
        file->write(data, 11);

        char buffer[11];
        file->read(buffer, 11);
        driver::debugcon::WriteString(buffer, 11);
    }
}