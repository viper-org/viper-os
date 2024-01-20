#include <syscall/proc/spawn.h>

#include <sched/sched.h>

namespace echis
{
    namespace syscall
    {
        int spawn(const char* path)
        {
            sched::Process* proc = sched::Process::Create(path);
            proc->copyFileDescriptionsFrom(sched::Current()->getParent());

            sched::AddProcess(proc);

            return proc->getPid();
        }
    }
}