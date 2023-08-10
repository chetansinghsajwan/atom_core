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
            auto Set()
                requires(RInvokable<TInvokable, TResult(TArgs...)>)
            {
                _impl = [](void* obj, TResult& result, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    new (&result) TResult(invokable(fwd(args)...));
                };
            }

            auto Invoke(void* invokable, TArgs&&... args) -> TResult
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
            auto Set()
            {
                _impl = [](void* obj, TArgs&&... args)
                {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                    invokable(fwd(args)...);
                };
            }

            auto Invoke(void* invokable, TArgs&&... args)
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
    class InvokableBox <TResult(TArgs...)> : public
        ObjectBox<true, true, true, 50, DefaultMemAllocator>,
        public Private::InvokableBoxIdentifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() { }

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null):
            ObjectBox(null) { }

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(NullType null) -> InvokableBox&
        {
            ObjectBox::operator=(null);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator==(NullType null) const -> bool
        {
            return ObjectBox::operator==(null);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        InvokableBox(TInvokable&& invokable) 
            requires(RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>):
            ObjectBox(fwd(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TResult(TArgs...)> TInvokable>
        auto operator=(TInvokable&& invokable) -> InvokableBox&
            requires(RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>)
        {
            ObjectBox::operator=(fwd(invokable));
            _SetInvoker<TInvokable>();
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(const InvokableBox& other) :
            ObjectBox(other) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        auto operator=(const InvokableBox& other) -> InvokableBox&
        {
            ObjectBox::operator=(other);
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        InvokableBox(InvokableBox&& other) :
            ObjectBox(mov(other)) { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        auto operator=(InvokableBox&& other) -> InvokableBox&
        {
            ObjectBox::operator=(mov(other));
            return self;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        ~InvokableBox() { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        auto Invoke(TArgs&&... args) -> TResult
        {
            ATOM_ASSERT(ObjectBox::_HasObject()) << NullPointerException(
                "InvokableTarget is null.");

            return _invoker.Invoke(ObjectBox::_GetObject(), fwd(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        auto operator()(TArgs&&... args) -> TResult
        {
            return Invoke(fwd(args)...);
        }

        template <typename T>
        auto GetInvokable() -> T*
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return ObjectBox::_GetObject<T>();
        }

        auto GetInvokableType() const -> const TypeInfo&
        {
            return ObjectBox::_GetObjectType();
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        auto _SetInvoker()
            requires(RInvokable<TInvokable, TResult(TArgs...)>)
        {
            _invoker.template Set<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        auto _InvokeInvokable(TArgs&&... args) -> TResult
        {
            return _invoker.Invoke(fwd(args)...);
        }

        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker _invoker;
    };
}