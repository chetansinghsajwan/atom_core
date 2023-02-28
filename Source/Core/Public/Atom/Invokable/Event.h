#pragma once
#include "Atom/Containers.h"
#include "Atom/Invokable/InvokableBox.h"
#include <functional>

namespace Atom
{
    struct EventKey
    {
    public:
        EventKey(const TypeInfo& typeInfo) noexcept:
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
    /// EventSource is used to manage listeners and dispatch event.
    /// 
    /// @TODO Add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class EventSource
    {
    public:
        constexpr EventSource() noexcept { }

    public:
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        EventKey operator += (TInvokable&& listener) noexcept
        {
            return Subscribe(FORWARD(listener));
        }

        bool operator -= (EventKey key) noexcept
        {
            return Unsubscribe(key);
        }

    public:
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        EventKey Subscribe(TInvokable&& listener) noexcept
        {
            return _AddListener(FORWARD(listener));
        }

        bool Unsubscribe(EventKey key) noexcept
        {
            return _RemoveListener(key);
        }

        void Dispatch(TArgs... args)
        {
            for (auto& listener : _listeners)
            {
                listener(args...);
            }
        }

    protected:
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        EventKey _AddListener(TInvokable&& listener)
        {
            _listeners.emplace_back(FORWARD(listener));
            return EventKey(_listeners.back().GetInvokableType());
        }

        bool _RemoveListener(EventKey key) noexcept
        {
            for (auto it = _listeners.begin(); it != _listeners.end(); it++)
            {
                if (it->GetInvokableType() == key.GetType())
                {
                    _listeners.erase(it);
                    return true;
                }
            }

            return false;
        }

        bool _HasListener(EventKey key) noexcept
        {
            for (auto it = _listeners.begin(); it != _listeners.end(); it++)
            {
                if (it->GetInvokableType() == key.GetType())
                {
                    return true;
                }
            }

            return false;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// List of event listeners.
        /// ----------------------------------------------------------------------------------------
        DynamicArray<InvokableBox<void(TArgs...)>> _listeners;
    };

    /// --------------------------------------------------------------------------------------------
    /// Event is just a frontend to EventSource to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class Event
    {
    public:
        constexpr Event(EventSource<TArgs...>& source) noexcept:
            _source(&source) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        EventKey operator += (TInvokable&& listener) noexcept
        {
            return Subscribe(FORWARD(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        bool operator -= (EventKey key) noexcept
        {
            return Unsubscribe(key);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TInvokable>
            requires RInvokable<TInvokable, void(TArgs...)>::Value
        EventKey Subscribe(TInvokable&& listener) noexcept
        {
            return _source->Subscribe(FORWARD(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        bool Unsubscribe(EventKey key) noexcept
        {
            return _source->Unsubscribe(MOVE(key));
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// EventSource object handling events.
        /// ----------------------------------------------------------------------------------------
        EventSource<TArgs...>* _source;
    };
}