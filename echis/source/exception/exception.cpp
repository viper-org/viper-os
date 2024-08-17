#include <exception/exception.h>

#include <driver/debugcon.h>

#include <mm/vm/lazyMap.h>

#include <atheris/cpu/halt.h>

namespace echis
{
    namespace exception
    {
        BaseException::BaseException(Exception e)
            : mE(e)
        {
        }

        Exception BaseException::getType() const
        {
            return mE;
        }

        AccessViolationException::AccessViolationException(std::uint64_t address)
            : BaseException(Exception::ACCESS_VIOLATION)
            , mFaultingAddress(address)
        {
        }

        std::uint64_t AccessViolationException::getFaultingAddress() const
        {
            return mFaultingAddress;
        }


        void OnException(BaseException* e)
        {
            if (e->getType() == Exception::ACCESS_VIOLATION)
            {
                auto faultingAddress = static_cast<AccessViolationException*>(e)->getFaultingAddress();
                if (vm::ShouldLazyMap(faultingAddress)) driver::debugcon::Write("should map\n");
                else driver::debugcon::Write("should not map\n");
            }

            driver::debugcon::WriteFormatted("Received exception: %s\n", e->getName());
            atheris::cpu::Halt();
        }
    }
}