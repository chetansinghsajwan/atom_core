#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Memory/ObjectBox.h"
#include "Atom/String.h"
#include "Atom/TTI.h"

#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    namespace Private
    {
        class InvokableBoxIdentifier
        {};

        template <typename TResult, typename... TArgs>
        class Invoker
        {
        public:
            template <typename TInvokable>
            auto Set()
                requires(RInvokable<TInvokable, TResult(TArgs...)>)
            {
                _impl = [](MemPtr<void> obj, TResult& result, TArgs&&... args) {
                    TInvokable& invokable = *static_cast<TInvokable*>(obj.unwrap());
                    new (&result) TResult(invokable(forward<TArgs>(args)...));
                };
            }

            auto Invoke(MemPtr<void> invokable, TArgs&&... args) -> TResult
            {
                TResult result;
                _impl(invokable, result, forward<TArgs>(args)...);

                return result;
            }

        protected:
            void (*_impl)(MemPtr<void> invokable, TResult& result, TArgs&&... args);
        };

        template <typename... TArgs>
        class Invoker<void, TArgs...>
        {
        public:
            template <RInvokable<void(TArgs...)> TInvokable>
            auto Set()
            {
                _impl = [](MemPtr<void> obj, TArgs&&... args) {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj.unwrap());
                    invokable(forward<TArgs>(args)...);
                };
            }

            auto Invoke(MemPtr<void> invokable, TArgs&&... args)
            {
                _impl(invokable, forward<TArgs>(args)...);
            }

        protected:
            void (*_impl)(MemPtr<void> invokable, TArgs&&... args);
        };
    }

    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class InvokableBox;

    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TResult, typename... TArgs>
    class InvokableBox<TResult(TArgs...)>
        : public ObjectBox<true, true, true, 50, DefaultMemAllocator>
        , public Private::InvokableBoxIdentifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() {}

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null)
            : ObjectBox(null)
        {}

        /// ----------------------------------------------------------------------------------------
        /// NullAssignmentOperator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(NullType null) -> InvokableBox&
        {
            ObjectBox::operator=(null);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// NullEqualityOperator.
        /// ----------------------------------------------------------------------------------------
        auto eq(NullType null) const -> bool
        {
            return ObjectBox::eq(null);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        requires RInvokable<TInvokable, TResult(TArgs...)>
        InvokableBox(TInvokable&& invokable)
            requires(RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>)
            : ObjectBox(forward<TInvokable>(invokable))
        {
            _SetInvoker<TInvokable>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        requires RInvokable<TInvokable, TResult(TArgs...)>
        auto operator=(TInvokable&& invokable) -> InvokableBox& requires(
            RNotDerivedFrom<TInvokable, Private::InvokableBoxIdentifier>) {
            ObjectBox::operator=(forward<TInvokable>(invokable));
            _SetInvoker<TInvokable>();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        InvokableBox(const InvokableBox& other)
            : ObjectBox(other)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(const InvokableBox& other) -> InvokableBox&
        {
            ObjectBox::operator=(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        InvokableBox(InvokableBox&& other)
            : ObjectBox(mov(other))
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(InvokableBox&& other) -> InvokableBox&
        {
            ObjectBox::operator=(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ~InvokableBox() {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto Invoke(TArgs&&... args) -> TResult
        {
            Contracts::Expects(ObjectBox::_HasObject(), "InvokableTarget is null.");

            return _invoker.Invoke(ObjectBox::_GetObject(), forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator()(TArgs&&... args) -> TResult
        {
            return Invoke(forward<TArgs>(args)...);
        }

        template <typename T>
        auto GetInvokable() -> MemPtr<T>
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
            return _invoker.Invoke(forward<TArgs>(args)...);
        }

        using TInvoker = Private::Invoker<TResult, TArgs...>;

        TInvoker _invoker;
    };
}
