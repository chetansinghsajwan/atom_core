#pragma once
#include "Atom/Memory/ObjectBox.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"
#include "Atom/Str.h"

#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        struct InvokableBoxIdentifier { };

        template <tname TResult, tname... TArgs>
        struct Invoker
        {
            template <tname TInvokable>
            requires RInvokable<TInvokable, TResult(TArgs...)>
            fn Set()
            {
                _impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(fwd(args)...));
                };
            }

            fn Invoke(void* invokable, TArgs&&... args) -> TResult
            {
                TResult result;
                _impl(invokable, result, fwd(args)...);

                return result;
            }

        protected:
            void (*_impl) (void* invokable, TResult& result, TArgs&&... args);
        };

        template <tname... TArgs>
        struct Invoker <void, TArgs...>
        {
            template <RInvokable<void(TArgs...)> TInvokable>
            fn Set()
            {
                _impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(fwd(args)...);
                };
            }

            fn Invoke(void* invokable, TArgs&&... args)
            {
                _impl(invokable, fwd(args)...);
            }

        protected:
            void (*_impl) (void* invokable, TArgs&&... args);
        };
    }

    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <tname>
    class InvokableBox;

    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TResult, tname... TArgs>
    class InvokableBox <TResult(TArgs...)> :
        public ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor InvokableBox() noex { }

        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(NullType null) noex :
            ObjectBox(null) { }

        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        fn operator = (NullType null) -> InvokableBox&
        {
            ObjectBox::operator = (null);
            return *this;
        }

        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        bool operator == (NullType null) const noex
        {
            return ObjectBox::operator == (null);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        ctor InvokableBox(TInvokable&& invokable) :
            ObjectBox(fwd(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
            requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        fn operator = (TInvokable&& invokable) -> InvokableBox&
        {
            ObjectBox::operator = (fwd(invokable));
            _SetInvoker<TInvokable>();
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(const InvokableBox& other) :
            ObjectBox(other) { }

        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator = (const InvokableBox& other) -> InvokableBox&
        {
            ObjectBox::operator = (other);
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(InvokableBox&& other) :
            ObjectBox(MOVE(other)) { }

        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator = (InvokableBox&& other) -> InvokableBox&
        {
            ObjectBox::operator = (MOVE(other));
            return *this;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        dtor InvokableBox() { }

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        fn Invoke(TArgs&&... args) -> TResult
        {
            ATOM_ASSERT(ObjectBox::_HasObject()) << NullPointerException(
                TEXT("InvokableTarget is null."));

            return _invoker.Invoke(ObjectBox::_GetObject(), fwd(args)...);
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator () (TArgs&&... args) -> TResult
        {
            return Invoke(fwd(args)...);
        }

    public:
        template <tname T>
        fn GetInvokable() noex -> T*
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        fn GetInvokableType() const noex -> const TypeInfo&
        {
            return ObjectBox::_GetObjectType();
        }

    protected:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        fn _SetInvoker()
        {
            _invoker.template Set<TInvokable>();
        }

        fn _InvokeInvokable(TArgs&&... args) -> TResult
        {
            return _invoker.Invoke(fwd(args)...);
        }

    protected:
        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker _invoker;
    };
}