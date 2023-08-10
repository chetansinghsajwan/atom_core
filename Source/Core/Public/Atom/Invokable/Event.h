#pragma once
#include "Atom/Containers.h"
#include "Atom/Invokable/InvokableBox.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// EventKey is used to identify events registered for this key.
    /// --------------------------------------------------------------------------------------------
    class EventKey
    {
    public:
        ctor EventKey(const TypeInfo& typeInfo):
            _typeInfo(typeInfo) { }

    public:
        fn GetType() const -> const TypeInfo&
        {
            return _typeInfo;
        }

    private:
        const TypeInfo& _typeInfo;
    };

    /// --------------------------------------------------------------------------------------------
    /// {Event} is just a frontend to {EventSource} to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class IEvent
    {
    protected:
        using _TSignature = void(TArgs...);

    public:
        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(fwd(listener));
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        fn operator+=(TInvokable&& listener) -> EventKey
            requires(RInvokable<TInvokable, _TSignature>)
        {
            return Subscribe(fwd(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key);
        /// ----------------------------------------------------------------------------------------
        fn operator-=(EventKey key) -> bool
        {
            return Unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(fwd(listener)) on {Source}.
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
        fn Subscribe(TInvokable&& listener) -> EventKey
            requires(RInvokable<TInvokable, _TSignature>)
        {
            return Subscribe(InvokableBox<_TSignature>(fwd(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Subscribe(InvokableBox<_TSignature>&& invokable) -> EventKey abstract;

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key) on {Source}.
        /// ----------------------------------------------------------------------------------------
        virtual fn Unsubscribe(EventKey key) -> usize abstract;
    };

    /// --------------------------------------------------------------------------------------------
    /// EventSource is used to manage listeners and dispatch event.
    /// 
    /// @TODO Add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class EventSource extends IEvent<TArgs...>
    {
    protected:
        using _TSignature = typename IEvent<TArgs...>::_TSignature;
        using _TListener = InvokableBox<_TSignature>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Subscribe(InvokableBox<_TSignature>&& invokable) -> EventKey ofinal
        {
            return _AddListener(fwd(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Unsubscribe(EventKey key) -> usize ofinal
        {
            return _RemoveListener(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dispatches the events. Calls each event listener(invokables) with given args.
        /// 
        /// @TODO Add detailed documentation on argument passing.
        /// ----------------------------------------------------------------------------------------
        fn Dispatch(TArgs... args)
        {
            for (auto& listener : _listeners)
            {
                listener(args...);
            }
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _AddListener(InvokableBox<_TSignature>&& invokable) -> EventKey
        {
            EventKey key = invokable.GetInvokableType();

            _listeners.InsertBack(mov(invokable));
            return key;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _RemoveListener(EventKey key) -> usize
        {
            return RangeModifier().RemoveIf(_listeners, [&](const auto& listener)
                {
                    return listener.GetInvokableType() == key.GetType();
                });
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _CountListeners(EventKey key) -> usize
        {
            usize count = 0;
            for (auto& listener : _listeners)
            {
                if (listener.GetInvokableType() == key.GetType())
                {
                    count++;
                }
            }

            return count;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// List of event listeners.
        /// ----------------------------------------------------------------------------------------
        DynArr<_TListener> _listeners;
    };
}