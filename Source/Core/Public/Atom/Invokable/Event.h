#pragma once
#include "Atom/Containers.h"
#include "Atom/Invokable/InvokableBox.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// SEventKey is used to identify events registered for this key.
    /// --------------------------------------------------------------------------------------------
    struct SEventKey
    {
    public:
        SEventKey(const TypeInfo& typeInfo) noexcept:
            _typeInfo(typeInfo) { }

    public:
        const TypeInfo& GetType() const noexcept
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
    struct IEvent
    {
        using TSignature = void(TArgs...);

        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(FORWARD(listener));
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TSignature> TInvokable>
        SEventKey operator += (TInvokable&& listener) noexcept
        {
            return Subscribe(FORWARD(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key);
        /// ----------------------------------------------------------------------------------------
        bool operator -= (SEventKey key) noexcept
        {
            return Unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(FORWARD(listener)) on {Source}.
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<TSignature> TInvokable>
        SEventKey Subscribe(TInvokable&& listener) noexcept
        {
            return Subscribe(InvokableBox<TSignature>(FORWARD(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual SEventKey Subscribe(InvokableBox<TSignature>&& invokable) noexcept = 0;

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key) on {Source}.
        /// ----------------------------------------------------------------------------------------
        virtual usize Unsubscribe(SEventKey key) noexcept = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// EventSource is used to manage listeners and dispatch event.
    /// 
    /// @TODO Add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class EventSource: public IEvent<TArgs...>
    {        
    public:
        constexpr EventSource() noexcept { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual SEventKey Subscribe(InvokableBox<void(TArgs...)>&& invokable) noexcept override final
        {
            return _AddListener(FORWARD(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual usize Unsubscribe(SEventKey key) noexcept override final
        {
            return _RemoveListener(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Dispatches the events. Calls each event listener(invokables) with given args.
        /// 
        /// @TODO Add detailed documentation on argument passing.
        /// ----------------------------------------------------------------------------------------
        void Dispatch(TArgs... args)
        {
            for (auto& listener : _listeners.Iterator())
            {
                listener(args...);
            }
        }

    protected:
        SEventKey _AddListener(InvokableBox<void(TArgs...)>&& invokable)
        {
            SEventKey key = invokable.GetInvokableType();

            _listeners.InsertBack(MOVE(invokable));
            return key;
        }

        usize _RemoveListener(SEventKey key) noexcept
        {
            return _listeners.RemoveIf([&](const auto& listener)
                {
                    return listener.GetInvokableType() == key.GetType();
                });
        }

        usize _CountListeners(SEventKey key) noexcept
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
    
        /// ----------------------------------------------------------------------------------------
        /// List of event listeners.
        /// ----------------------------------------------------------------------------------------
        DynamicArray<TListener> _listeners;
    };
}