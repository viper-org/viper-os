#include <syscall/mem/mmap.h>

#include <mm/vmm.h>
#include <mm/pmm.h>

#include <util/math.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        void* mmap(size_t length)
        {
            size_t size = util::AlignUp(length / pmm::GetPageSize(), pmm::GetPageSize());
            return vm::GetPages(&sched::Current()->getParent()->getAddressSpace(),
                                size,
                                atheris::vm::flags::present | atheris::vm::flags::write | atheris::vm::flags::user);
        }
    }
}