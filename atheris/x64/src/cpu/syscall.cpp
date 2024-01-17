#include <cpu/syscall.h>
#include <cpu/asm.h>

#include <syscall/file/read.h>
#include <syscall/file/write.h>
#include <syscall/file/open.h>

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
                    default:
                        printf("%#%s", 0x00ffff, frame->rdi);
                        break;
                }
            }
        }
    }
}