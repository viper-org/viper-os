#include <cpu/interrupt/Exception.h>
#include <cpu/interrupt/Interrupt.h>

#include <drivers/Terminal.h>

#include <container/list.h>
#include <container/function.h>

#include <memory.h>

namespace exception
{
    [[noreturn]] void ExceptionPanic(InterruptFrame* frame);

    vpr::list<Handler>* handlers;

    void Init()
    {
        handlers = new vpr::list<Handler>[32];
    }
    
    void subscribe(int vector, Handler callback)
    {
        handlers[vector].push_back(std::move(callback));
    }

    extern "C" void CommonExceptionHandler(InterruptFrame* frame)
    {
        if (handlers[frame->baseFrame.vector].size())
        {
            for (const auto& handler : handlers[frame->baseFrame.vector])
            {
                if (handler(frame)) // Nonzero return value means failure
                {
                    ExceptionPanic(frame);
                }
            }
        }
    }

    constexpr const char* exceptionMessages[] = {
        "Division by Zero #DE",
        "Debug #DB",
        "Non-maskable Interrupt",
        "Breakpoint #BP",
        "Overflow #OF",
        "Bound Range Exceeded #BR",
        "Invalid Opcode #UD",
        "Device Not Available #NM",
        "Double Fault #DF",
        "Coprocessor Segment Overrun",
        "Invalid TSS #TS",
        "Segment Not Present #NP",
        "Stack-Segment Fault #SS",
        "General Protection Fault #GP",
        "Page Fault #PF",
        "Reserved",
        "x87 Floating-Point Exception #MF",
        "Alignment Check #AC",
        "Machine Check #MC",
        "SIMD Floating-Point Exception #XM",
        "Virtualization Exception #VE",
        "Control Protection Exception #CP",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Hypervisor Injection Exception #HV",
        "VMM Communication Exception #VC",
        "Security Exception #SX",
        "Reserved"
    };

    void ExceptionPanic(InterruptFrame* frame)
    {
        Terminal::Print(exceptionMessages[frame->baseFrame.vector]);

        for(;;)
        {
            asm volatile("1: cli; hlt; jmp 1b");
        }
    }
}