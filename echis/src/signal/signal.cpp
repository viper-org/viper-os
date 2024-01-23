#include <signal/signal.h>

#include <mm/pmm.h>

#include <sched/sched.h>

#include <atheris/sched/signal.h>

#include <atheris/mm/vm.h>

namespace echis
{
    namespace signal
    {
        void DeliverToCurrent(Signal signal)
        {
            for (auto& signalObject : sched::Current()->getPendingSignals())
            {
                if (signalObject.signum == Signal::NONE)
                {
                    signalObject.signum = signal;
                    return;
                }
            }
            sched::Current()->getPendingSignals().push_back(PendingSignalObject{signal});
        }

        PendingSignalObject* CheckIncoming()
        {
            sched::Thread* thread = sched::Current();
            if (!thread)
            {
                return nullptr;
            }

            for (auto& signal : thread->getPendingSignals())
            {
                if (signal.signum != Signal::NONE)
                {
                    uint64_t top = 0x7fff'ffff'e000;
                    atheris::vm::MapPage(&thread->getParent()->getAddressSpace(),
                                         pmm::GetPage(),
                                         top - pmm::GetPageSize(),
                                         atheris::vm::flags::present | atheris::vm::flags::write | atheris::vm::flags::user);

                    thread->getSignalHandlerStack().size = pmm::GetPageSize();
                    thread->getSignalHandlerStack().top = top;
                    return &signal;
                }
            }
            return nullptr;
        }

        void(*GetHandler(PendingSignalObject* signal))(int)
        {
            return sched::Current()->getSignalHandler(signal->signum);
        }
    }
}