#pragma once
#include "atom/core/core/core.h"

namespace atom
{
    class result_void
    {};

    /// --------------------------------------------------------------------------------------------
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    template <typename in_impl_t>
    class result_api
    {
        template <typename that_impl_t>
        friend class result_api;

    private:
        template <typename that_t>
        struct is_result_api
        {
            static constexpr bool value = false;
        };

        template <typename that_impl_t>
        struct is_result_api<result_api<that_impl_t>>
        {
            static constexpr bool value = true;
        };

    private:
        using this_t = result_api;

    protected:
        using impl_t = in_impl_t;

    public:
        using value_t = typename impl_t::out_value_t;
        using value_type_info_t = typeinfo<value_t>;
        using error_types_list = typename impl_t::error_types_list;
        using first_error_t = typename impl_t::first_error_t;

    public:
        static_assert(value_type_info_t::is_pure, "non pure value type are not supported.");

        static_assert(value_type_info_t::is_void or value_type_info_t::is_destructible,
            "non destructible value type is not supported.");

        static_assert(error_types_list::count > 0, "there must be at least 1 error type.");

        static_assert(error_types_list::are_unique(), "each error type must be unique.");

        static_assert(error_types_list::are_all([](auto info) { return info.is_pure; }),
            "non pure error types are not supported.");

        static_assert(error_types_list::are_all([](auto info) { return info.is_destructible; }),
            "non destructible error types are not supported.");

    public:
        static constexpr bool should_enable_copy_constructor = [] {
            if (not value_type_info_t::is_copy_constructible)
                return false;

            if (not error_types_list::are_all([](auto info) { return info.is_copy_constructible; }))
                return false;

            if (value_type_info_t::is_copy_constructible
                and error_types_list::are_all(
                    [](auto info) { return info.is_trivially_copy_constructible; }))
                return false;

            return true;
        };

        template <typename in_that_t>
        static constexpr bool should_enable_universal_copy_constructor = [] {
            using that_t = typeinfo<in_that_t>::pure_t;

            if constexpr (not is_result_api<that_t>::value)
                return false;

            else if (not value_type_info_t::template is_constructible_from<
                         typename that_t::value_t>)
                return false;

            else if (not error_types_list::has_all(typename that_t::error_types_list()))
                return false;

            else if (not that_t::error_types_list::are_all(
                         [](auto info) { return info.is_copy_constructible; }))
                return false;

            return true;
        }();

        static constexpr bool should_enable_copy_operator = [] {
            if (not value_type_info_t::is_copyable)
                return false;

            if (not error_types_list::are_all([](auto info) { return info.is_copyable; }))
                return false;

            if (value_type_info_t::is_trivially_copyable
                and error_types_list::are_all([](auto info) { return info.is_trivially_copyable; }))
                return false;

            return true;
        }();

        template <typename in_that_t>
        static constexpr bool should_enable_universal_copy_operator = [] {
            using that_t = typeinfo<in_that_t>::pure_t;

            if constexpr (not is_result_api<that_t>::value)
                return false;

            else if (not value_type_info_t::template is_constructible_from<typename that_t::value_t>
                     or not value_type_info_t::template is_assignable_from<
                         typename that_t::value_t>)
                return false;

            else if (not error_types_list::has_all(typename that_t::error_types_list()))
                return false;

            else if (not error_types_list::are_all([](auto info) { return info.is_copyable; }))
                return false;

            return true;
        }();

        static constexpr bool should_enable_move_constructor = [] {
            if (not value_type_info_t::is_move_constructible)
                return false;

            if (not error_types_list::are_all([](auto info) { return info.is_move_constructible; }))
                return false;

            if (value_type_info_t::is_trivially_move_constructible
                and error_types_list::are_all(
                    [](auto info) { return info.is_trivially_move_constructible; }))
                return false;

            return true;
        }();

        template <typename in_that_t>
        static constexpr bool should_enable_universal_move_constructor = [] {
            using that_t = typeinfo<in_that_t>::pure_t;

            if constexpr (not is_result_api<that_t>::value)
                return false;

            else if (not value_type_info_t::template is_constructible_from<
                         typename that_t::value_t>)
                return false;

            else if (not error_types_list::has_all(typename that_t::error_types_list()))
                return false;

            else if (not that_t::error_types_list::are_all(
                         [](auto info) { return info.is_move_constructible; }))
                return false;

            return true;
        }();

        static constexpr bool should_enable_move_operator = [] {
            if (not value_type_info_t::is_moveable)
                return false;

            if (not error_types_list::are_all([](auto info) { return info.is_moveable; }))
                return false;

            if (value_type_info_t::is_trivially_moveable
                and error_types_list::are_all([](auto info) { return info.is_trivially_moveable; }))
                return false;

            return true;
        }();

        template <typename in_that_t>
        static constexpr bool should_enable_universal_move_operator = [] {
            using that_t = typeinfo<in_that_t>::pure_t;

            if constexpr (not is_result_api<that_t>::value)
                return false;

            else if (not value_type_info_t::template is_constructible_from<typename that_t::value_t>
                     or not value_type_info_t::template is_assignable_from<
                         typename that_t::value_t>)
                return false;

            else if (not error_types_list::has_all(typename that_t::error_types_list()))
                return false;

            else if (not that_t::error_types_list::are_all(
                         [](auto info) { return info.is_moveable; }))
                return false;

            return true;
        }();

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        static consteval auto has_error() -> bool
        {
            return impl_t::template has_error<error_t>();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor (deleted)
        /// ----------------------------------------------------------------------------------------
        constexpr result_api() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result_api(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result_api(const this_t& that)
            requires should_enable_copy_constructor
            : _impl(typename impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal copy constructor
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr result_api(const that_t& that)
            requires should_enable_universal_copy_constructor<that_t>
            : _impl(typename impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(const this_t& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(const this_t& that)
            requires should_enable_copy_operator
        {
            _impl.set_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr this_t& operator=(const that_t& that)
            requires should_enable_universal_copy_operator<that_t>
        {
            _impl.set_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result_api(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result_api(this_t&& that)
            requires should_enable_move_constructor
            : _impl(typename impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr result_api(that_t&& that)
            requires should_enable_universal_move_constructor<that_t>
            : _impl(typename impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(this_t&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(this_t&& that)
            requires should_enable_move_operator
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal move operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_t>
        constexpr this_t& operator=(that_t&& that)
            requires should_enable_universal_move_operator<that_t>
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # void value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result_api(result_void)
            requires value_type_info_t::is_void
            : _impl(typename impl_t::value_tag(), {})
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs result_api with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr result_api(const this_t::value_t& value)
            requires(not value_type_info_t::is_void)
            : _impl(typename impl_t::value_tag(), value)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs result_api with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr result_api(this_t::value_t&& value)
            requires(not value_type_info_t::is_void)
            : _impl(typename impl_t::value_tag(), move(value))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value sets new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr this_t& operator=(const this_t::value_t& value)
            requires(not value_type_info_t::is_void)
        {
            _impl.set_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// destroys previous value sets new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr this_t& operator=(this_t::value_t&& value)
            requires(not value_type_info_t::is_void)
        {
            _impl.set_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # error copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr result_api(const error_t& error)
            requires(has_error<typename typeinfo<error_t>::pure_t>())
            : _impl(typename impl_t::error_tag(), error)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # error move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr result_api(error_t&& error)
            requires(has_error<typename typeinfo<error_t>::pure_t>())
            : _impl(typename impl_t::error_tag(), move(error))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # error copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr this_t& operator=(const error_t& error)
            requires(has_error<typename typeinfo<error_t>::pure_t>())
        {
            _impl.set_error(error);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # error move operator
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr this_t& operator=(error_t&& error)
            requires(has_error<typename typeinfo<error_t>::pure_t>())
        {
            _impl.set_error(move(error));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~result_api() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... args_t>
        constexpr auto emplace_value(this this_t& this_, args_t&&... args) -> void
        {
            this_._impl.emplace_value(forward<args_t>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr auto set_value(this this_t& this_, const this_t::value_t& value) -> void
            requires(not value_type_info_t::is_void)
        {
            this_._impl.set_value(value);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr auto set_value(this this_t& this_, this_t::value_t&& value) -> void
            requires(not value_type_info_t::is_void)
        {
            this_._impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set_value(this this_t& this_) -> void
            requires value_type_info_t::is_void
        {
            this_._impl.set_value_void();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr auto get_value(this const this_t& this_) -> const this_t::value_t&
            requires(not value_type_info_t::is_void)
        {
            return this_._impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_t = this_t>
        constexpr auto get_value(this this_t& this_) -> this_t::value_t&
            requires(not value_type_info_t::is_void)
        {
            return this_._impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value(this const this_t& this_) -> bool
        {
            return this_._impl.is_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_error_count(this const this_t& this_) -> usize
        {
            return this_._impl.get_error_count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t, typename... args_t>
        constexpr auto emplace_error(this this_t& this_, args_t&&... args) -> void
            requires(has_error<error_t>())
        {
            this_._impl.template emplace_error<error_t>(forward<args_t>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr auto set_error(this this_t& this_, const error_t& error) -> void
            requires(has_error<error_t>())
        {
            this_._impl.set_error(error);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr auto set_error(this this_t& this_, error_t&& error) -> void
            requires(has_error<error_t>())
        {
            this_._impl.set_error(move(error));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr auto get_error(this const this_t& this_) -> const error_t&
            requires(has_error<error_t>())
        {
            return this_._impl.template get_error<error_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr auto get_error(this this_t& this_) -> error_t&
            requires(has_error<error_t>())
        {
            return this_._impl.template get_error<error_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_error(this const this_t& this_) -> const first_error_t&
            requires(get_error_count() == 1)
        {
            return this_._impl.get_first_error();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_error(this this_t& this_) -> first_error_t&
            requires(get_error_count() == 1)
        {
            return this_._impl.get_first_error();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_error(this const this_t& this_) -> bool
        {
            return this_._impl.is_error();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_t>
        constexpr auto is_error(this const this_t& this_) -> bool
            requires(has_error<error_t>())
        {
            return this_._impl.template is_error<error_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto panic_on_error(this const this_t& this_) -> void
        {
            this_._impl.panic_on_error();
        }

    protected:
        impl_t _impl;
    };
}
