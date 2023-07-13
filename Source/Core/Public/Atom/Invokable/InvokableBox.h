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
        class InvokableBoxIdentifier { };

        template <tname TResult, tname... TArgs>
        class Invoker
        {
            pub template <tname TInvokable>
            requires RInvokable<TInvokable, TResult(TArgs...)>
            fn Set()
            {
                _impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(fwd(args)...));
                };
            }

            pub fn Invoke(void* invokable, TArgs&&... args) -> TResult
            {
                TResult result;
                _impl(invokable, result, fwd(args)...);

                return result;
            }

        protected:
            void (*_impl) (void* invokable, TResult& result, TArgs&&... args);
        };

        template <tname... TArgs>
        class Invoker <void, TArgs...>
        {
            pub template <RInvokable<void(TArgs...)> TInvokable>
            fn Set()
            {
                _impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(fwd(args)...);
                };
            }

            pub fn Invoke(void* invokable, TArgs&&... args)
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
    class InvokableBox <TResult(TArgs...)> extends
        ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor InvokableBox() noex { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        pub ctor InvokableBox(NullType null) noex :
            ObjectBox(null) { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        pub fn operator = (NullType null) -> InvokableBox&
        {
            ObjectBox::operator = (null);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        pub fn operator == (NullType null) const noex -> bool
        {
            return ObjectBox::operator == (null);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <RInvokable<TResult(TArgs...)> TInvokable>
        requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        ctor InvokableBox(TInvokable&& invokable) :
            ObjectBox(fwd(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <RInvokable<TResult(TArgs...)> TInvokable>
        requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        fn operator = (TInvokable&& invokable) -> InvokableBox&
        {
            ObjectBox::operator = (fwd(invokable));
            _SetInvoker<TInvokable>();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub ctor InvokableBox(const InvokableBox& other) :
            ObjectBox(other) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub fn operator = (const InvokableBox& other) -> InvokableBox&
        {
            ObjectBox::operator = (other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub ctor InvokableBox(InvokableBox&& other) :
            ObjectBox(MOVE(other)) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub fn operator = (InvokableBox&& other) -> InvokableBox&
        {
            ObjectBox::operator = (MOVE(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub dtor InvokableBox() { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub fn Invoke(TArgs&&... args) -> TResult
        {
            ATOM_ASSERT(ObjectBox::_HasObject()) << NullPointerException(
                TEXT("InvokableTarget is null."));

            return _invoker.Invoke(ObjectBox::_GetObject(), fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub fn operator () (TArgs&&... args) -> TResult
        {
            return Invoke(fwd(args)...);
        }

        pub template <tname T>
        fn GetInvokable() noex -> T*
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        pub fn GetInvokableType() const noex -> const TypeInfo&
        {
            return ObjectBox::_GetObjectType();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pro template <tname TInvokable>
        requires RInvokable<TInvokable, TResult(TArgs...)>
        fn _SetInvoker()
        {
            _invoker.template Set<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pro fn _InvokeInvokable(TArgs&&... args) -> TResult
        {
            return _invoker.Invoke(fwd(args)...);
        }

        pro using TInvoker = Private::Invoker<TResult, TArgs...>;

        pro TInvoker _invoker;
    };
}