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
        pub ctor EventKey(const TypeInfo& typeInfo) noex:
            _typeInfo(typeInfo) { }

        pub fn GetType() const noex -> const TypeInfo&
        {
            return _typeInfo;
        }

        priv const TypeInfo& _typeInfo;
    };

    /// --------------------------------------------------------------------------------------------
    /// {Event} is just a frontend to {EventSource} to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <tname... TArgs>
    class IEvent
    {
        prot using _TSignature = void(TArgs...);

        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(fwd(listener));
        /// ----------------------------------------------------------------------------------------
        pub template <tname TInvokable>
        requires RInvokable<TInvokable, _TSignature>
        fn operator += (TInvokable&& listener) noex -> EventKey
        {
            return Subscribe(fwd(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key);
        /// ----------------------------------------------------------------------------------------
        pub fn operator -= (EventKey key) noex -> bool
        {
            return Unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(fwd(listener)) on {Source}.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TInvokable>
        requires RInvokable<TInvokable, _TSignature>
        fn Subscribe(TInvokable&& listener) noex -> EventKey
        {
            return Subscribe(InvokableBox<_TSignature>(fwd(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub virtual fn Subscribe(InvokableBox<_TSignature>&& invokable) noex -> EventKey abstract;

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key) on {Source}.
        /// ----------------------------------------------------------------------------------------
        pub virtual fn Unsubscribe(EventKey key) noex -> usize abstract;
    };

    /// --------------------------------------------------------------------------------------------
    /// EventSource is used to manage listeners and dispatch event.
    /// 
    /// @TODO Add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <tname... TArgs>
    class EventSource extends IEvent<TArgs...>
    {
        prot using _TSignature = tname IEvent<TArgs...>::_TSignature;
        
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub virtual fn Subscribe(InvokableBox<_TSignature>&& invokable) noex -> EventKey ofinal
        {
            return _AddListener(fwd(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub virtual fn Unsubscribe(EventKey key) noex -> usize ofinal
        {
            return _RemoveListener(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dispatches the events. Calls each event listener(invokables) with given args.
        /// 
        /// @TODO Add detailed documentation on argument passing.
        /// ----------------------------------------------------------------------------------------
        pub fn Dispatch(TArgs... args)
        {
            for (auto& listener : _listeners)
            {
                listener(args...);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        prot fn _AddListener(InvokableBox<_TSignature>&& invokable) -> EventKey
        {
            EventKey key = invokable.GetInvokableType();

            _listeners.InsertBack(MOVE(invokable));
            return key;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        prot fn _RemoveListener(EventKey key) noex -> usize
        {
            return RangeModifier().RemoveIf(_listeners, [&](const auto& listener)
                {
                    return listener.GetInvokableType() == key.GetType();
                });
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        prot fn _CountListeners(EventKey key) noex -> usize
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

        prot using TListener = InvokableBox<_TSignature>;

        /// ----------------------------------------------------------------------------------------
        /// List of event listeners.
        /// ----------------------------------------------------------------------------------------
        prot DynArr<TListener> _listeners;
    };
}