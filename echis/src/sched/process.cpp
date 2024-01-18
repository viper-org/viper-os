#include <sched/process.h>

#include <mm/pmm.h>
#include <mm/vmm.h>

#include <ldr/elf.h>

#include <atheris/sched/user.h>

namespace echis
{
    namespace sched
    {
        Thread::Thread()
        {
        }

        Thread::Thread(Process& parent, uint64_t start)
            : mParent(&parent)
            , mStart(start)
        {
        }

        Process* Thread::getParent() const
        {
            return mParent;
        }

        uint64_t Thread::getStartingAddress() const
        {
            return mStart;
        }

        void Thread::setStartingAddress(uint64_t newStart)
        {
            mStart = newStart;
        }

        Thread::Stack& Thread::getUserStack()
        {
            return mUserStack;
        }

        Thread::Stack& Thread::getKernelStack()
        {
            return mKernelStack;
        }

        atheris::sched::ThreadContext*& Thread::getContext()
        {
            return mContext;
        }


        Process::Process()
        {
        }

        Process::Process(uint64_t startAddress)
            : mMainThread(Thread(*this, startAddress))
            , mAddressSpace(atheris::vm::AddressSpace::Create())
        {
        }

        int Process::getPid() const
        {
            return mPid;
        }

        Thread* Process::getMainThread()
        {
            return &mMainThread;
        }
        
        atheris::vm::AddressSpace& Process::getAddressSpace()
        {
            return mAddressSpace;
        }

        FileDescriptor& Process::getFd(int n)
        {
            return mFds[n];
        }

        int Process::addOpenFileDescription(fs::vfs::Node* node, OpenMode::OpenMode mode)
        {
            for (int i = 0; i < MAX_FD; ++i)
            {
                if (mFds[i].vfsNode == nullptr && mFds[i].pipe == nullptr)
                {
                    mFds[i].vfsNode = node;
                    mFds[i].pipe    = nullptr;
                    mFds[i].flags   = mode;
                    return i;
                }
            }
            return -1;
        }

        int Process::addOpenFileDescription(vpr::shared_ptr<Pipe> pipe)
        {
            for (int i = 0; i < MAX_FD; ++i)
            {
                if (mFds[i].vfsNode == nullptr && mFds[i].pipe == nullptr)
                {
                    mFds[i].pipe    = pipe;
                    mFds[i].vfsNode = nullptr;
                    mFds[i].flags   = 0;
                    return i;
                }
            }
            return -1;
        }

        int Process::closeFileDescription(int fd)
        {
            if (mFds[fd].vfsNode == nullptr)
            {
                return -1;
            }
            mFds[fd].vfsNode->close();
            mFds[fd].flags = 0;
            mFds[fd].vfsNode = nullptr;
            if (mFds[fd].pipe)
            {
                mFds[fd].pipe = nullptr;
            }
            return 0;
        }


        Process* Process::Create(const char* path)
        {
            Process* proc = new Process(0);

            fs::vfs::Node* node = fs::vfs::lookup(path);
            char* buffer = new char[node->size()];
            size_t count = node->size();
            node->read(buffer, &count, 0);

            elf::Executable exec = elf::Load(buffer, &proc->getAddressSpace());
            proc->getMainThread()->setStartingAddress(exec.entryPoint);

            delete[] buffer;

            pmm::physaddr stackPage = pmm::GetPage();
            atheris::vm::MapPage(&proc->getAddressSpace(),
                                stackPage,
                                0xFFFFFF0000000000,
                                atheris::vm::flags::present | atheris::vm::flags::write);
            proc->getMainThread()->getKernelStack().size = pmm::GetPageSize();
            proc->getMainThread()->getKernelStack().top  = 0xFFFFFF0000000000 + pmm::GetPageSize();

            proc->getMainThread()->getUserStack().size = pmm::GetPageSize();
            proc->getMainThread()->getUserStack().top  = reinterpret_cast<uint64_t>(vm::GetPages(&proc->getAddressSpace(),
                                                                    1,
                                                                    atheris::vm::flags::write | atheris::vm::flags::user)) + pmm::GetPageSize();

            return proc;
        }
    }
}