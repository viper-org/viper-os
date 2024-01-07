#ifndef VIPEROS_ECHIS_CORE_EXCEPTION_EXCEPTION_H
#define VIPEROS_ECHIS_CORE_EXCEPTION_EXCEPTION_H 1

namespace echis
{
    namespace exception
    {
        /*
         * Enum of types of arch-independent
         * exceptions
         *
         * DIV - Divison error
         * SEG - Memory access violation
         * ILL - Illegal instruction
         * BRK - Break/trap or other debug exception
         * ARC - Other architecture specific exception
         */
        enum Exception
        {
            DIV,
            SEG,
            ILL,
            BRK,
            ARC
        };

        /*
         * Raise an exception for the appropriate
         * handler to be called
         *
         * code - an exception specific error code
         * to be passed on to the handler
         */
        void raise(Exception except, int code);
    }
}

#endif // VIPEROS_ECHIS_CORE_EXCEPTION_EXCEPTION_H