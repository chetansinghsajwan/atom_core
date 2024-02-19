export module atom.core:event;
import :std;
import :core;
import :invokable;
import :invokable_box;
import :containers.dynamic_array;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename... event_arg_types>
    class _event_source_impl
    {
    public:
        using signature_type = void(event_arg_types...);
        using key_type = const std::type_info&;

    public:
        template <typename invokable_type>
        auto add_listener(invokable_type&& invokable) -> key_type
        {
            invokable_box<signature_type> box(forward<invokable_type>(invokable));
            key_type key = box.get_type();

            _listeners.emplace_back(move(box));
            return key;
        }

        auto remove_listener(key_type key) -> usize
        {
            return _listeners.remove_if(
                [&](const auto& listener) { return listener.get_type() == key; });
        }

        auto count_listeners(key_type key) -> usize
        {
            usize count = 0;
            for (auto& listener : _listeners)
            {
                if (listener.get_type() == key)
                {
                    count++;
                }
            }

            return count;
        }

        auto dispatch(event_arg_types... args) -> void
        {
            for (auto& listener: _listeners)
            {
                listener.invoke(args...);
            }
        }

    private:
        dynamic_array<invokable_box<signature_type>> _listeners;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
export namespace atom
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
    template <typename... event_arg_types>
    class event_source
    {
        using _impl_type = _event_source_impl<event_arg_types...>;

    public:
        using signature_type = void(event_arg_types...);
        using key_type = event_key;

    public:
        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)` on the source.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto subscribe(invokable_type&& listener) -> event_key
            requires(rinvokable<invokable_type, signature_type>)
        {
            return event_key(_impl.add_listener(forward<invokable_type>(listener)));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto operator+=(invokable_type&& listener) -> event_key
            requires(rinvokable<invokable_type, signature_type>)
        {
            return subscribe(forward<invokable_type>(listener));
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
        auto dispatch(event_arg_types... args)
        {
            _impl.dispatch(forward<event_arg_types>(args)...);
        }

    private:
        _impl_type _impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// this type is just a frontend to [`event_source`] to prevent users from dispatching events.
    /// --------------------------------------------------------------------------------------------
    template <typename... event_arg_types>
    class event_source_view
    {
    public:
        using source_type = event_source<event_arg_types...>;
        using signature_type = typename source_type::signature_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        event_source_view(source_type& source)
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
        template <typename invokable_type>
        auto subscribe(invokable_type&& listener) -> event_key
            requires(rinvokable<invokable_type, signature_type>)
        {
            return _source.subscribe(forward<invokable_type>(listener));
        }

        /// ----------------------------------------------------------------------------------------
        /// calls `subscribe(listener)`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        auto operator+=(invokable_type&& listener) -> event_key
            requires(rinvokable<invokable_type, signature_type>)
        {
            return subscribe(forward<invokable_type>(listener));
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
        source_type& _source;
    };
}
