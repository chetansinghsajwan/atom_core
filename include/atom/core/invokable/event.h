#pragma once
// #include "atom/core/_std.h"
// #include "atom/core/core.h"
#include "atom/core/invokable/invokable_box.h"
#include "atom/core/containers/dynamic_array.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename... event_arg_ts>
    class _event_source_impl
    {
    public:
        using signature_t = void(event_arg_ts...);
        using key_t = const std::type_info&;

    public:
        template <typename invokable_t>
        auto add_listener(invokable_t&& invokable) -> key_t
        {
            invokable_box<signature_t> box(forward<invokable_t>(invokable));
            key_t key = box.get_t();

            _listeners.emplace_back(move(box));
            return key;
        }

        auto remove_listener(key_t key) -> usize
        {
            return _listeners.remove_one_if(
                [&](const auto& listener) { return listener.get_t() == key; });
        }

        auto count_listeners(key_t key) -> usize
        {
            usize count = 0;
            for (auto& listener : _listeners)
            {
                if (listener.get_t() == key)
                {
                    count++;
                }
            }

            return count;
        }

        auto dispatch(event_arg_ts... args) -> void
        {
            for (auto& listener : _listeners)
            {
                listener.invoke(args...);
            }
        }

    private:
        dynamic_array<invokable_box<signature_t>> _listeners;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// event_key is used to identify events registered for this key.
    /// --------------------------------------------------------------------------------------------
    class event_key
    {
    public:
        event_key(const std::type_info& info)
            : _info(info)
        {}

    public:
        const std::type_info& _info;
    };

    /// --------------------------------------------------------------------------------------------
    /// event_source is used to manage listeners and dispatch event.
    /// --------------------------------------------------------------------------------------------
    template <typename... event_arg_ts>
    class event_source
    {
        using _impl_t = _event_source_impl<event_arg_ts...>;

    public:
        using signature_t = void(event_arg_ts...);
        using key_t = event_key;

    public:
        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)` on the source.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto subscribe(invokable_t&& listener) -> event_key
            requires typeinfo<invokable_t>::template is_invokable<signature_t>
        {
            return event_key(_impl.add_listener(forward<invokable_t>(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto operator+=(invokable_t&& listener) -> event_key
            requires typeinfo<invokable_t>::template is_invokable<signature_t>
        {
            return subscribe(forward<invokable_t>(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls unsubscribe(key) on {source}.
        /// ----------------------------------------------------------------------------------------
        virtual auto unsubscribe(event_key key) -> usize
        {
            return _impl.remove_listener(key._info);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `unsubscribe(key)`.
        /// ----------------------------------------------------------------------------------------
        auto operator-=(event_key key) -> bool
        {
            return unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// dispatches the events. calls each event listener(invokables) with given args.
        /// ----------------------------------------------------------------------------------------
        auto dispatch(event_arg_ts... args)
        {
            _impl.dispatch(forward<event_arg_ts>(args)...);
        }

    private:
        _impl_t _impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// this type is just a frontend to [`event_source`] to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <typename... event_arg_ts>
    class event_source_view
    {
    public:
        using source_t = event_source<event_arg_ts...>;
        using signature_t = typename source_t::signature_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view(source_t& source)
            : _source(source)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view(const event_source_view& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view& operator=(const event_source_view& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view(event_source_view&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view& operator=(event_source_view&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~event_source_view() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)` on the source.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto subscribe(invokable_t&& listener) -> event_key
            requires typeinfo<invokable_t>::template is_invokable<signature_t>
        {
            return _source.subscribe(forward<invokable_t>(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto operator+=(invokable_t&& listener) -> event_key
            requires typeinfo<invokable_t>::template is_invokable<signature_t>
        {
            return subscribe(forward<invokable_t>(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls unsubscribe(key) on {source}.
        /// ----------------------------------------------------------------------------------------
        virtual auto unsubscribe(event_key key) -> usize
        {
            return _source.unsubscribe(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `unsubscribe(key)`.
        /// ----------------------------------------------------------------------------------------
        auto operator-=(event_key key) -> bool
        {
            return unsubscribe(key);
        }

    private:
        source_t& _source;
    };
}
