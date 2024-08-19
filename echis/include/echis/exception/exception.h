#ifndef VIPEROS_ECHIS_EXCEPTION_EXCEPTION_H
#define VIPEROS_ECHIS_EXCEPTION_EXCEPTION_H 1

#include <cstdint>

namespace echis
{
    namespace exception
    {
        enum class Exception
        {
            ACCESS_VIOLATION,

            ARCH_SPECIFIC
        };

        class BaseException
        {
        public:
            BaseException(Exception e);
            virtual ~BaseException() { }

            Exception getType() const;

            constexpr const char* getName() const
            {
                switch(mE)
                {
                    case Exception::ACCESS_VIOLATION:
                        return "ACCESS_VIOLATION";
                    case Exception::ARCH_SPECIFIC:
                        return "ARCH_SPECIFIC";
                }
                return ""; // unreachable
            }

        protected:
            Exception mE;
        };

        class AccessViolationException : public BaseException
        {
        public:
            AccessViolationException(std::uint64_t address);

            std::uint64_t getFaultingAddress() const;

        private:
            std::uint64_t mFaultingAddress;
        };

        void OnException(BaseException* e);
    }
}

#endif // VIPEROS_ECHIS_EXCEPTION_EXCEPTION_H