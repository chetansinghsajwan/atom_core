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

        template <typename TResult, typename... TArgs>
        class Invoker
        {
        public:
            template <typename TInvokable>
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

        template <typename... TArgs>
        class Invoker <void, TArgs...>
        {
        public:
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
    template <typename>
    class InvokableBox;

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TResult, typename... TArgs>
    class InvokableBox <TResult(TArgs...)> extends
        ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor InvokableBox() { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(NullType null):
            ObjectBox(null) { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        fn operator=(NullType null) -> InvokableBox&
        {
            ObjectBox::operator=(null);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        fn operator==(NullType null) const -> bool
        {
            return ObjectBox::operator==(null);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        ctor InvokableBox(TInvokable&& invokable) :
            ObjectBox(fwd(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        requires RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>
        fn operator=(TInvokable&& invokable) -> InvokableBox&
        {
            ObjectBox::operator=(fwd(invokable));
            _SetInvoker<TInvokable>();
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(const InvokableBox& other) :
            ObjectBox(other) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator=(const InvokableBox& other) -> InvokableBox&
        {
            ObjectBox::operator=(other);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        ctor InvokableBox(InvokableBox&& other) :
            ObjectBox(mov(other)) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator=(InvokableBox&& other) -> InvokableBox&
        {
            ObjectBox::operator=(mov(other));
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        dtor InvokableBox() { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn Invoke(TArgs&&... args) -> TResult
        {
            ATOM_ASSERT(ObjectBox::_HasObject()) << NullPointerException(
                "InvokableTarget is null.");

            return _invoker.Invoke(ObjectBox::_GetObject(), fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn operator()(TArgs&&... args) -> TResult
        {
            return Invoke(fwd(args)...);
        }

        template <typename T>
        fn GetInvokable() -> T*
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        fn GetInvokableType() const -> const TypeInfo&
        {
            return ObjectBox::_GetObjectType();
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        requires RInvokable<TInvokable, TResult(TArgs...)>
        fn _SetInvoker()
        {
            _invoker.template Set<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _InvokeInvokable(TArgs&&... args) -> TResult
        {
            return _invoker.Invoke(fwd(args)...);
        }

        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker _invoker;
    };
}