export module atom_core:core.variant;

import :types;
import :contracts;
import :core.core;
import :core.nums;
import :core.variant_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// `variant` template type is derived from `this` type, this helps to identify if a type is
    /// `variant` template type.
    /// --------------------------------------------------------------------------------------------
    class variant_tag
    {};

    /// --------------------------------------------------------------------------------------------
    /// this type is used to represent a type safe union. it stores a value of one of the
    /// `value_types` at any given moment.
    /// --------------------------------------------------------------------------------------------
    export template <typename... value_types>
    class variant: public variant_tag
    {
        template <typename... that_value_types>
        friend class variant;

    private:
        using this_type = variant<value_types...>;
        using impl_type = variant_impl<value_types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// `type_list` of value types of `this` variant.
        /// ----------------------------------------------------------------------------------------
        using value_types_list = type_list<value_types...>;

    public:
        static_assert(value_types_list::get_count() > 1, "at least two type must to be specified.");
        static_assert(value_types_list::are_pure(), "every type should be pure.");
        static_assert(value_types_list::are_unique(), "every type should be unique.");
        static_assert(value_types_list::are_destructible_or_void(),
            "every type should be destructible or void.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const this_type& that)
            requires(value_types_list::are_copy_constructible()
                     and not value_types_list::are_trivially_copy_constructible())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(const variant<that_value_types...>& that)
            requires(type_list<that_value_types...>::are_copy_constructible()
                     and value_types_list::template has_all<that_value_types...>())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that)
            requires(value_types_list::are_copyable()
                     and not value_types_list::are_trivially_copy_assignable())
        {
            _impl.set_value_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr this_type& operator=(const variant<that_value_types...>& that)
            requires(value_types_list::are_copyable()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.set_value_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(this_type&& that)
            requires(value_types_list::are_move_constructible()
                     and not value_types_list::are_trivially_move_constructible())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(variant<that_value_types...>&& that)
            requires(type_list<that_value_types...>::are_move_constructible()
                     and value_types_list::template has_all<that_value_types...>())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that)
            requires(value_types_list::are_moveable()
                     and not value_types_list::are_trivially_move_assignable())
        {
            _impl.set_value_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr this_type& operator=(variant<that_value_types...>&& that)
            requires(value_types_list::are_moveable()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.set_value_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// this constructor is used when this doesn't support all of `that_value_types` but some
        /// of them. if the value is of some type that `this` supports, it constructs `this` with
        /// that value, else panics.
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(create_from_variant_tag, const variant<that_value_types...>& that)
            requires(value_types_list::template has_any<that_value_types...>())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(create_from_variant_tag, variant<that_value_types...>&& that)
            requires(value_types_list::template has_any<that_value_types...>())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// creates value in place with `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr variant(create_by_emplace_tag<value_type>, arg_types&&... args)
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::template is_constructible_from<arg_types...>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{},
                forward<arg_types>(args)... }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// if this supports void type, then sets stored type to void.
        /// ----------------------------------------------------------------------------------------
        constexpr variant(create_by_emplace_tag<void>)
            requires(value_types_list::template has<void>())
            : _impl{ typename impl_type::template emplace_tag<void>{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(create_from_void_tag)
            requires(value_types_list::template has<void>())
            : _impl{ typename impl_type::template emplace_tag<void>{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// constructs `this` by copying `value` into `this`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(const value_type& value)
            requires(value_types_list::template has<value_type>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// constructs `this` by moving `value` into `this`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(value_type&& value)
            requires(value_types_list::template has<value_type>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, move(value) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// if the stored value is of type `value_type` then assigns it, else destroys stored value
        /// and constructs new value with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr this_type& operator=(const value_type& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.set_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr this_type& operator=(value_type&& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.set_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// destroys stored value if any, even if the value if of same type as `value_type`
        /// and creates a new value in place of type `value_type` with `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> void
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            _impl.template emplace_value<value_type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// if the stored value is of same type as `value_type`, assigns `value` to it, else
        /// destroys stored value and constructs a new value of type `value_type` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set(const value_type& value) -> void
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::is_copy_constructible())
        {
            _impl.set_value(value);
        }

        template <typename value_type>
        constexpr auto set(value_type&& value) -> void
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::is_move_constructible())
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        template <typename value_type>
        constexpr auto get() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return move(_impl.template get_value<value_type>());
        }

        template <typename value_type>
        constexpr auto get() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_checked() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        template <typename value_type>
        constexpr auto get_checked() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return move(_impl.template get_value<value_type>());
        }

        template <typename value_type>
        constexpr auto get_checked() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value<value_type>();
        }

        template <typename value_type>
        constexpr auto get_unchecked() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return move(_impl.template get_value<value_type>());
        }

        template <typename value_type>
        constexpr auto get_unchecked() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the stored value is of type `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto is() const -> bool
            requires(value_types_list::template has<value_type>())
        {
            return _impl.template is_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the stored value is of any of the types `that_value_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr auto is_any() const -> bool
            requires(value_types_list::template has_all<that_value_types...>())
        {
            return _impl.template is_any_type<that_value_types...>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the index of the currently stored type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_index() const -> usize
        {
            return _impl.get_index();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `that` holds the same type as `this` does and then compares those
        /// values.
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr auto operator==(const variant<that_value_types...>& that) const -> bool
            requires(value_types_list::are_equality_comparable()
                     and value_types_list::template has_any<that_value_types...>())
        {
            return _impl.is_eq(that._impl);
        }

    private:
        impl_type _impl;
    };
}
