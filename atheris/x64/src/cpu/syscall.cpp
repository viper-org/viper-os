#include <cpu/syscall.h>
#include <cpu/asm.h>

#include <syscall/file/read.h>
#include <syscall/file/write.h>
#include <syscall/file/open.h>
#include <syscall/file/close.h>
#include <syscall/file/ioctl.h>
#include <syscall/file/seek.h>

#include <syscall/mem/mmap.h>

#include <syscall/proc/pipe.h>

#include <stdio.h>

namespace atheris
{
    namespace x64
    {
        namespace cpu
        {
            constexpr int SCE = (1 << 0);

            extern "C" void syscall_handler();

            void EnableSyscall()
            {
                uint64_t efer = ReadMSR(MSR::IA32_EFER);
                efer |= SCE;
                WriteMSR(MSR::IA32_EFER,  efer);
                WriteMSR(MSR::IA32_STAR,  ((uint64_t)0x8 << 32) | ((uint64_t)0x13 << 48));
                WriteMSR(MSR::IA32_LSTAR, reinterpret_cast<uint64_t>(syscall_handler));
            }

            extern "C" void syscall_dispatcher(SyscallFrame* frame)
            {
                switch(frame->rax) // rax contains the syscall number
                {
                    case 0: // read
                    {
                        frame->rax = echis::syscall::read(frame->rdi, reinterpret_cast<void*>(frame->rsi), frame->rdx);
                        break;
                    }
                    case 1: //write
                    {
                        frame->rax = echis::syscall::write(frame->rdi, reinterpret_cast<const void*>(frame->rsi), frame->rdx);
                        break;
                    }
                    case 2: // open
                    {
                        frame->rax = echis::syscall::open(reinterpret_cast<const char*>(frame->rdi), frame->rsi);
                        break;
                    }
                    case 3: // close
                    {
                        frame->rax = echis::syscall::close(frame->rdi);
                        break;
                    }
                    case 4: // ioctl
                    {
                        frame->rax = echis::syscall::ioctl(frame->rdi, frame->rsi, reinterpret_cast<char*>(frame->rdx));
                        break;
                    }
                    case 5: // mmap
                    {
                        frame->rax = reinterpret_cast<uint64_t>(echis::syscall::mmap(frame->rdi));
                        break;
                    }
                    case 6: // seek
                    {
                        frame->rax = echis::syscall::seek(frame->rdi, frame->rsi);
                        break;
                    }
                    case 7: // pipe
                    {
                        frame->rax = echis::syscall::pipe(reinterpret_cast<int*>(frame->rdi));
                        break;
                    }
                    default:
                        printf("%#%s", 0x00ffff, frame->rdi);
                        break;
                }
            }
        }
    }
}