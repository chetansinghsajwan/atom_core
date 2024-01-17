#pragma once
#include "atom/containers.h"
#include "atom/invokable/invokable_box.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// event_key is used to identify events registered for this key.
    /// --------------------------------------------------------------------------------------------
    class event_key
    {
    public:
        event_key(const type_info& type_info)
            : _type_info(type_info)
        {}

    public:
        auto get_type() const -> const type_info&
        {
            return _type_info;
        }

    private:
        const type_info& _type_info;
    };

    /// --------------------------------------------------------------------------------------------
    /// [`event`] is just a frontend to [`event_source`] to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    class ievent
    {
    protected:
        using _tsignature = void(arg_types...);

    public:
        /// ----------------------------------------------------------------------------------------
        /// calls subscribe(forward<tinvokable>(listener));
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
        auto operator+=(tinvokable&& listener) -> event_key
            requires(rinvokable<tinvokable, _tsignature>)
        {
            return subscribe(forward<tinvokable>(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls unsubscribe(key);
        /// ----------------------------------------------------------------------------------------
        auto operator-=(event_key key) -> bool
        {
            return unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls subscribe(forward<tinvokable>(listener)) on {source}.
        /// ----------------------------------------------------------------------------------------
        template <typename tinvokable>
        auto subscribe(tinvokable&& listener) -> event_key
            requires(rinvokable<tinvokable, _tsignature>)
        {
            return subscribe(invokable_box<_tsignature>(forward<tinvokable>(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto subscribe(invokable_box<_tsignature>&& invokable) -> event_key = 0;

        /// ----------------------------------------------------------------------------------------
        /// calls unsubscribe(key) on {source}.
        /// ----------------------------------------------------------------------------------------
        virtual auto unsubscribe(event_key key) -> usize = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// event_source is used to manage listeners and dispatch event.
    ///
    /// @todo add async dispatching.
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    class event_source: public ievent<arg_types...>
    {
    protected:
        using _tsignature = typename ievent<arg_types...>::_tsignature;
        using _tlistener = invokable_box<_tsignature>;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto subscribe(invokable_box<_tsignature>&& invokable) -> event_key override final
        {
            return _add_listener(mov(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        virtual auto unsubscribe(event_key key) -> usize override final
        {
            return _remove_listener(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// dispatches the events. calls each event listener(invokables) with given args.
        ///
        /// @todo add detailed documentation on argument passing.
        /// ----------------------------------------------------------------------------------------
        auto dispatch(arg_types... args)
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
        auto _add_listener(invokable_box<_tsignature>&& invokable) -> event_key
        {
            event_key key = invokable.get_invokable_type();

            _listeners.emplace_back(mov(invokable));
            return key;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _remove_listener(event_key key) -> usize
        {
            return _listeners.remove_if(
                [&](const auto& listener) { return listener.get_invokable_type() == key.get_type(); });
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto _count_listeners(event_key key) -> usize
        {
            usize count = 0;
            for (auto& listener : _listeners)
            {
                if (listener.get_invokable_type() == key.get_type())
                {
                    count++;
                }
            }

            return count;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// list of event listeners.
        /// ----------------------------------------------------------------------------------------
        dynamic_array<_tlistener> _listeners;
    };
}
