#pragma once
#include "Atom/Core.h"
#include "Atom/Memory.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"
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
    template <typename TResult, typename... TArgs>
    class InvokableBox <TResult(TArgs...)>:
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
        template <typename TInvokable>
            requires RInvokable<TInvokable, TResult(TArgs...)>::Value
        InvokableBox(TInvokable&& invokable):
            ObjectBox(FORWARD(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
            requires RInvokable<TInvokable, TResult(TArgs...)>::Value
        InvokableBox& operator = (TInvokable&& invokable)
        {
            ObjectBox::operator = (FORWARD(invokable));
            _SetInvoker<TInvokable>();
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
        TResult Invoke(TArgs&&... args)
        {
            ASSERT<NullPointerException>(ObjectBox::_HasObject(),
                TEXT("InvokableTarget is null."));

            return _invoker.Invoke(ObjectBox::_GetObject(), FORWARD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        TResult operator () (TArgs&&... args)
        {
            return Invoke(FORWARD(args)...);
        }

    public:
        template <typename T>
        T* GetInvokable() noexcept
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        const TypeInfo& GetInvokableType() const noexcept
        {
            return ObjectBox::_GetObjectType();
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
            requires RInvokable<TInvokable, TResult(TArgs...)>::Value
        void _SetInvoker()
        {
            _invoker.template Set<TInvokable>();
        }

        TResult _InvokeInvokable(TArgs&&... args)
        {
            return _invoker.Invoke(FORWARD(args)...);
        }

    protected:
        template <typename>
        struct Invoker
        {
            template <typename TInvokable>
                requires RInvokable<TInvokable, TResult(TArgs...)>::Value
            void Set()
            {
                _impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(FORWARD(args)...));
                };
            }

            TResult Invoke(void* invokable, TArgs&&... args)
            {
                TResult result;
                _impl(invokable, result, FORWARD(args)...);

                return result;
            }

        protected:
            void (*_impl) (void* invokable, TResult& result, TArgs&&... args);
        };

        template <>
        struct Invoker <void>
        {
            template <typename TInvokable>
                requires RInvokable<TInvokable, TResult(TArgs...)>::Value
            void Set()
            {
                _impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(FORWARD(args)...);
                };
            }

            TResult Invoke(void* invokable, TArgs&&... args)
            {
                _impl(invokable, FORWARD(args)...);
            }

        protected:
            void (*_impl) (void* invokable, TArgs&&... args);
        };

        Invoker<TResult> _invoker;
    };
}