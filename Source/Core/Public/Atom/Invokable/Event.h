#pragma once
#include "Atom/Containers.h"
#include "Atom/Invokable/InvokableBox.h"
#include <functional>

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
        /// ----------------------------------------------------------------------------------------
        /// Calls Subscribe(FORWARD(listener));
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
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
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        SEventKey Subscribe(TInvokable&& listener) noexcept
        {
            return Subscribe(InvokableBox<void(TArgs...)>(FORWARD(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        virtual SEventKey Subscribe(InvokableBox<void(TArgs...)>&& invokable) noexcept = 0;

        /// ----------------------------------------------------------------------------------------
        /// Calls Unsubscribe(key) on {Source}.
        /// ----------------------------------------------------------------------------------------
        virtual SizeT Unsubscribe(SEventKey key) noexcept = 0;
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
        virtual SizeT Unsubscribe(SEventKey key) noexcept override final
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
            for (auto& listener : _listeners)
            {
                listener(args...);
            }
        }

    protected:
        SEventKey _AddListener(InvokableBox<void(TArgs...)>&& invokable)
        {
            _listeners.push_back(MOVE(invokable));
            return SEventKey(_listeners.back().GetInvokableType());
        }

        SizeT _RemoveListener(SEventKey key) noexcept
        {
            for (auto it = _listeners.begin(); it != _listeners.end(); it++)
            {
                if (it->GetInvokableType() == key.GetType())
                {
                    _listeners.erase(it);
                    return 1;
                }
            }

            return 0;
        }

        SizeT _CountListeners(SEventKey key) noexcept
        {
            SizeT count = 0;
            for (auto it = _listeners.begin(); it != _listeners.end(); it++)
            {
                if (it->GetInvokableType() == key.GetType())
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
        DynamicArray<InvokableBox<void(TArgs...)>> _listeners;
    };
}