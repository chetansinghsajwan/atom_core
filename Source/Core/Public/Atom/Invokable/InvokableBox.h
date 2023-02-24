#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Exceptions.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class InvokableBox;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename Result, typename... Args>
    class InvokableBox <Result(Args...)>:
        public ObjectBox<50, DefaultMemAllocator>
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null) noexcept:
            ObjectBox(null) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (NullType null)
        {
            ObjectBox::operator = (null);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        bool operator == (NullType null) const noexcept
        {
            ObjectBox::operator == (null);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename InvokableType>
            requires Invokable<InvokableType, Result, Args...>
        InvokableBox(InvokableType&& invokable):
            ObjectBox(FORWARD(invokable))
        {
            _SetInvoker<InvokableType>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename InvokableType>
            requires Invokable<InvokableType, Result, Args...>
        InvokableBox& operator = (InvokableType&& invokable)
        {
            ObjectBox::operator = (FORWARD(invokable));
            _SetInvoker<InvokableType>();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(const InvokableBox& other):
            ObjectBox(other) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (const InvokableBox& other)
        {
            ObjectBox::operator = (other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(InvokableBox&& other):
            ObjectBox(MOVE(other)) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox& operator = (InvokableBox&& other)
        {
            ObjectBox::operator = (MOVE(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        ~InvokableBox() { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        Result Invoke(Args&&... args)
        {
            ASSERT<NullPointerException>(ObjectBox::_HasObject(),
                "InvokableTarget is null.");

            return _invoker.Invoke(ObjectBox::_GetObject(), FORWARD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        Result operator () (Args&&... args)
        {
            return Invoke(FORWARD(args)...);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename InvokableType>
            requires Invokable<InvokableType, Result, Args...>
        void _SetInvoker()
        {
            _invoker.template Set<InvokableType>();
        }

    protected:
        struct Invoker
        {
            template <typename InvokableType>
                requires Invokable<InvokableType, Result, Args...>
            void Set()
            {
                _impl = [](void* obj, Result& result, Args&&... args)
                {
                    InvokableType& invokable = *reinterpret_cast<InvokableType*>(obj);
                    result = invokable(FORWARD(args)...);
                };
            }

            Result Invoke(void* invokable, Args&&... args)
            {
                Result result;
                _impl(invokable, result, FORWARD(args)...);

                return result;
            }

        protected:
            void (*_impl) (void* invokable, Result& result, Args&&... args);
        };

        Invoker _invoker;
    };
}