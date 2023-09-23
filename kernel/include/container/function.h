#include "drivers/Terminal.h"
#ifndef VIPER_OS_CONTAINER_FUNCTION_H
#define VIPER_OS_CONTAINER_FUNCTION_H 1

#include <container/ptr.h>

#include <utility>

namespace vpr
{
    template <typename F>
    struct function;

    template <typename RetType, typename... Args>
    struct function<RetType(Args...)> {
        template <typename F>
        function(F f)
            : inner(vpr::make_unique<ConcreteFunction<F>>(std::move(f)))
            , innerPtr(nullptr)
        {
        }

        function(RetType(*ptr)(Args...))
            : innerPtr(ptr)
            , inner(nullptr)
        {
        }

        function(const function& rhs) 
            : innerPtr(rhs.innerPtr)
        {
            if (rhs.inner)
            {
                inner = rhs.inner->copy();
            }
        }
        function(function&& rhs) 
            : inner(std::move(rhs.inner))
            , innerPtr(std::move(rhs.innerPtr))
        {
        }

        function& operator=(const function& rhs)
        {
            if (rhs.inner)
            {
                inner = rhs.inner->copy();
            }
            innerPtr = rhs.innerPtr;
            return *this;
        }
        function& operator=(function&& rhs)
        {
            inner = std::move(rhs.inner);
            innerPtr = std::move(rhs.innerPtr);
            return *this;
        }

        RetType operator()(Args... args) const
        {
            if (inner)
            {
                return inner->call(static_cast<Args&&>(args)...);
            }
            return innerPtr(args...);
        }

    private:
        struct VirtualFunction
        {
            virtual RetType call(Args...) = 0;
            virtual vpr::unique_ptr<VirtualFunction> copy() const = 0;

            virtual ~VirtualFunction() = default;
        };

        template <typename F>
        struct ConcreteFunction : VirtualFunction
        {
            ConcreteFunction(F f) 
                : func(std::move(f))
            {
            }

            virtual RetType call(Args... args)
            {
                return func(static_cast<Args&&>(args)...);
            }
            virtual vpr::unique_ptr<VirtualFunction> copy() const
            {
                Terminal::Print("copy()\n");
                return vpr::make_unique<ConcreteFunction<F>>(func);
            }

            F func;
        };
        
        vpr::unique_ptr<VirtualFunction> inner;
        RetType(*innerPtr)(Args...);
    };
}

#endif