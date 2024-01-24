#include "atheris/private/sched/signal.h"
#include <cpu/syscall.h>
#include <cpu/asm.h>

#include <echis/signal/signal.h>

#include <syscall/file/read.h>
#include <syscall/file/write.h>
#include <syscall/file/open.h>
#include <syscall/file/close.h>
#include <syscall/file/ioctl.h>
#include <syscall/file/seek.h>
#include <syscall/file/poll.h>
#include <syscall/file/movfd.h>

#include <syscall/mem/mmap.h>

#include <syscall/proc/pipe.h>
#include <syscall/proc/spawn.h>
#include <syscall/proc/signal.h>
#include <syscall/proc/raise.h>
#include <syscall/proc/exit.h>
#include <syscall/proc/wait.h>

#include <syscall/info/sysconf.h>

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
                WriteMSR(MSR::IA32_SFMASK, 0x200); // Disable interrupts
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
                    case 8: // spawn
                    {
                        frame->rax = echis::syscall::spawn(reinterpret_cast<const char*>(frame->rdi));
                        break;
                    }
                    case 9: // poll
                    {
                        frame->rax = echis::syscall::poll(frame->rdi);
                        break;
                    }
                    case 10: // sigaction
                    {
                        frame->rax = reinterpret_cast<uint64_t>(echis::syscall::signal(frame->rdi, reinterpret_cast<echis::syscall::SignalHandler>(frame->rsi)));
                        break;
                    }
                    case 11: // raise
                    {
                        frame->rax = echis::syscall::raise(frame->rdi);
                        break;
                    }
                    case 12: // exit
                    {
                        echis::syscall::exit(frame->rdi);
                        break;
                    }
                    case 13: // wait
                    {
                        frame->rax = echis::syscall::wait(frame->rdi);
                        break;
                    }
                    case 14: // movfd
                    {
                        frame->rax = echis::syscall::movfd(frame->rdi, frame->rsi);
                        break;
                    }
                    case 15: // sysconf
                    {
                        frame->rax = echis::syscall::sysconf(frame->rdi);
                        break;
                    }
                    default:
                        printf("%#%s", 0x00ffff, frame->rdi);
                        break;
                }

                if (auto signal = echis::signal::CheckIncoming())
                {
                    sched::SavedUserContext context = {
                        frame->rip, frame->rsp, frame->rbp, frame->rflags,
                        frame->rax, frame->rbx, 0,          frame->rdi,
                        frame->rsi, frame->rdi, frame->r8, frame->r9,
                        frame->r10, 0,          frame->r12, frame->r13,
                        frame->r14, frame->r15
                    };
                    void(*handler)(int) = echis::signal::GetHandler(signal);
                    signal->signum = echis::signal::NONE;
                    sched::SignalHandler(context, handler);
                }
            }
        }
    }
}