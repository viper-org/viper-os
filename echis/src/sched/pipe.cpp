#include <sched/pipe.h>
#include <sched/sched.h>

#include <string.h>

namespace echis
{
    namespace sched
    {
        Pipe::Pipe(Pipe::Type type)
            : mType(type)
        {
        }

        Pipe::Type Pipe::getType() const
        {
            return mType;
        }


        WritePipe::WritePipe()
            : Pipe(Type::Write)
            , mBuffer(nullptr)
        {
        }

        size_t WritePipe::write(const void* buffer, size_t count)
        {
            if (mSize + count > mCapacity)
            {
                char* newBuffer = new char[mSize + count];
                if (mBuffer)
                {
                    memcpy(newBuffer, mBuffer, mSize + count);
                    delete mBuffer;
                }
                mBuffer = newBuffer;
                mCapacity = mSize;
            }
            memcpy(mBuffer + mSize, buffer, count);
            mSize += count;
            return count;
        }

        bool WritePipe::empty()
        {
            return mSize == 0;
        }


        ReadPipe::ReadPipe(WritePipe* writeEnd)
            : Pipe(Type::Read)
            , mWriteEnd(writeEnd)
        {
        }

        size_t ReadPipe::read(void* buffer, size_t count)
        {
            if (mWriteEnd->mSize < count)
            {
                count = mWriteEnd->mSize;
            }
            if (!mWriteEnd->mBuffer)
            {
                return 0;
            }
            memcpy(buffer, mWriteEnd->mBuffer, count);
            mWriteEnd->mSize -= count;
            for(size_t i = 0; i < mWriteEnd->mSize; ++i)
            {
                mWriteEnd->mBuffer[i] = mWriteEnd->mBuffer[i+count];
            }

            return count;
        }

        WritePipe* ReadPipe::getWriteEnd()
        {
            return mWriteEnd;
        }


        void CreatePipe(int fds[2])
        {
            WritePipe* write = new WritePipe;
            ReadPipe*  read  = new ReadPipe(write);

            fds[0] = sched::Current()->getParent()->addOpenFileDescription(read);
            fds[1] = sched::Current()->getParent()->addOpenFileDescription(write);
        }
    }
}