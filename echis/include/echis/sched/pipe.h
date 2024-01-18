#ifndef VIPEROS_ECHIS_SCHED_PIPE_H
#define VIPEROS_ECHIS_SCHED_PIPE_H 1

#include <fs/vfs.h>

#include <stdint.h>

namespace echis
{
    namespace sched
    {
        class Pipe
        {
        public:
            enum class Type
            {
                Read,
                Write
            };

            Pipe(Type type);

            Type getType() const;
            
        private:
            Type mType;
        };

        class WritePipe : public Pipe
        {
        friend class ReadPipe;
        public:
            WritePipe();

            size_t write(const void* buffer, size_t count);

        private:
            char* mBuffer;
            size_t mSize;
            size_t mCapacity;
        };

        class ReadPipe : public Pipe
        {
        public:
            ReadPipe(WritePipe* writeEnd);

            size_t read(void* buffer, size_t count);

        private:
            WritePipe* mWriteEnd;
        };

        void CreatePipe(int fds[2]);
    }
}

#endif // VIPEROS_ECHIS_SCHED_PIPE_H