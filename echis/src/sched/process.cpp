#include <sched/process.h>

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
    }
}