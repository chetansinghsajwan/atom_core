#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Memory/ObjectBox.h"
#include "Atom/String.h"
#include "Atom/TTI.h"

#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    class _InvokableBoxId
    {};

    template <typename TResult, typename... TArgs>
    class _Invoker
    {
    public:
        template <typename TInvokable>
        auto Set()
            requires(RInvokable<TInvokable, TResult(TArgs...)>)
        {
        public:
            template <typename TInvokable>
            auto Set()
                requires(RInvokable<TInvokable, TResult(TArgs...)>)
            {
                _impl = [](MutMemPtr<void> obj, TResult& result, TArgs&&... args) {
                    TInvokable& invokable = *static_cast<TInvokable*>(obj.unwrap());
                    new (&result) TResult(invokable(forward<TArgs>(args)...));
                };
            }

            auto Invoke(MutMemPtr<void> invokable, TArgs&&... args) -> TResult
            {
                TResult result;
                _impl(invokable, result, forward<TArgs>(args)...);

                return result;
            }

        protected:
            void (*_impl)(MutMemPtr<void> invokable, TResult& result, TArgs&&... args);
        };

        template <typename... TArgs>
        class Invoker<void, TArgs...>
        {
        public:
            template <RInvokable<void(TArgs...)> TInvokable>
            auto Set()
            {
                _impl = [](MutMemPtr<void> obj, TArgs&&... args) {
                    TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj.unwrap());
                    invokable(forward<TArgs>(args)...);
                };
            }

            auto Invoke(MutMemPtr<void> invokable, TArgs&&... args)
            {
                _impl(invokable, forward<TArgs>(args)...);
            }

        protected:
            void (*_impl)(MutMemPtr<void> invokable, TArgs&&... args);
        };
    }

    template <typename... TArgs>
    class _Invoker<void, TArgs...>
    {
    public:
        template <RInvokable<void(TArgs...)> TInvokable>
        auto Set()
        {
            _impl = [](void* obj, TArgs&&... args) {
                TInvokable& invokable = *reinterpret_cast<TInvokable*>(obj);
                invokable(fwd(args)...);
            };
        }

        auto Invoke(void* invokable, TArgs&&... args)
        {
            _impl(invokable, fwd(args)...);
        }

    protected:
        void (*_impl)(void* invokable, TArgs&&... args);
    };

    /// --------------------------------------------------------------------------------------------
    /// InvokableBox declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename>
    class InvokableBox;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TResult, typename... TArgs>
    class InvokableBox<TResult(TArgs...)>: public _InvokableBoxId
    {
    private:
        using _TBox = CopyMoveBox<void, true, 50>;
        using _TInvoker = _Invoker<TResult, TArgs...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr InvokableBox() {}

        /// ----------------------------------------------------------------------------------------
        /// NullConstructor.
        /// ----------------------------------------------------------------------------------------
        InvokableBox(NullType null):
            _box()
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
            requires(RNotDerivedFrom<TInvokable, _InvokableBoxId>)
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
        InvokableBox(const InvokableBox& other):
            _box(other)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(const InvokableBox& other) -> InvokableBox&
        {
            _box = other._box;
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        InvokableBox(InvokableBox&& other):
            _box(mov(other._box))
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator=(InvokableBox&& other) -> InvokableBox&
        {
            _box = mov(other._box);
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
            Contracts::Expects(_box.has(), "InvokableTarget is null.");

            return _invoker.Invoke(_box.mutMem(), forward<TArgs>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto operator()(TArgs&&... args) -> TResult
        {
            return Invoke(forward<TArgs>(args)...);
        }

        template <typename T>
        auto GetInvokable() -> MutMemPtr<T>
        {
            if (typeid(T) != GetInvokableType())
                return nullptr;

            return _box.mutMemAs<T>();
        }

        auto GetInvokableType() const -> const TypeInfo&
        {
            return _box.valType();
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

    private:
        _TInvoker _invoker;
        _TBox _box;
    };
}
