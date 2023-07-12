#pragma once
#include "Atom/Containers.h"
#include "Atom/Invokable/InvokableBox.h"

namespace Atom
{
    /// SEventKey is used to identify events registered for this key.
    /// --------------------------------------------------------------------------------------------
    struct SEventKey
    {
    public:
        ctor SEventKey(const TypeInfo& typeInfo) noex:
            _typeInfo(typeInfo) { }

    public:
        fn GetType() const noex -> const TypeInfo&
        {
            return _typeInfo;
        }

    private:
        const TypeInfo& _typeInfo;
    };

    /// {Event} is just a frontend to {EventSource} to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <tname... TArgs>
    struct IEvent
    {
        using TSignature = void(TArgs...);

        /// Calls Subscribe(fwd(listener));
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TSignature> TInvokable>
        fn operator += (TInvokable&& listener) noex -> SEventKey
        {
            return Subscribe(fwd(listener));
        }

        /// Calls Unsubscribe(key);
        /// ----------------------------------------------------------------------------------------
        fn operator -= (SEventKey key) noex -> bool
        {
            return Unsubscribe(key);
        }

        /// Calls Subscribe(fwd(listener)) on {Source}.
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TSignature> TInvokable>
        fn Subscribe(TInvokable&& listener) noex -> SEventKey
        {
            return Subscribe(InvokableBox<TSignature>(fwd(listener)));
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Subscribe(InvokableBox<TSignature>&& invokable) noex -> SEventKey abstract;

        /// Calls Unsubscribe(key) on {Source}.
        /// ----------------------------------------------------------------------------------------
        virtual fn Unsubscribe(SEventKey key) noex -> usize abstract;
    };

    /// EventSource is used to manage listeners and dispatch event.
    /// 
    /// @TODO Add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <tname... TArgs>
    class EventSource: public IEvent<TArgs...>
    {
    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Subscribe(InvokableBox<void(TArgs...)>&& invokable) noex -> SEventKey ofinal
        {
            return _AddListener(fwd(invokable));
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        virtual fn Unsubscribe(SEventKey key) noex -> usize ofinal
        {
            return _RemoveListener(key);
        }

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
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _AddListener(InvokableBox<void(TArgs...)>&& invokable) -> SEventKey
        {
            SEventKey key = invokable.GetInvokableType();

            _listeners.InsertBack(MOVE(invokable));
            return key;
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        fn _RemoveListener(SEventKey key) noex -> usize
        {
            return RangeModifier().RemoveIf(_listeners, [&](const auto& listener)
                {
                    return listener.GetInvokableType() == key.GetType();
                });
        }

        /// 
        /// ----------------------------------------------------------------------------------------
        fn _CountListeners(SEventKey key) noex -> usize
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
        using TListener = InvokableBox<void(TArgs...)>;

        /// List of event listeners.
        /// ----------------------------------------------------------------------------------------
        DynArr<TListener> _listeners;
    };
}