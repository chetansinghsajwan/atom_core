export module atom.core:core.variant;

import :types;
import :contracts;
import :core.core;
import :core.nums;
import :core.variant_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class variant_tag
    {};

    /// --------------------------------------------------------------------------------------------
    ///
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
        /// type_list of this variant.
        /// ----------------------------------------------------------------------------------------
        using value_types_list = type_list<value_types...>;

    public:
        static_assert(value_types_list::get_count() > 1, "at least two type must to be specified.");
        static_assert(value_types_list::are_pure(), "every type should be pure.");
        static_assert(value_types_list::are_unique(), "every type should be unique.");
        // static_assert(value_types_list::are_destructible(), "every type should be destructible.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const variant& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(const variant& that)
            requires(value_types_list::info_list::are_copy_constructible()
                     and not value_types_list::info_list::are_not_trivially_copy_constructible())
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
        constexpr auto operator=(const variant& that) -> variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(const variant& that)
            requires(value_types_list::are_copyable()
                     and not value_types_list::are_not_trivially_copy_assignable())
        {
            _impl.set_value_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant& operator=(const variant<that_value_types...>& that)
            requires(value_types_list::are_copyable()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.set_value_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(variant&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant(variant&& that)
            requires(value_types_list::are_move_constructible()
                     and value_types_list::are_not_trivially_move_constructible())
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
        constexpr auto operator=(variant&& that) -> variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr variant& operator=(variant&& that)
            requires(value_types_list::are_moveable()
                     and value_types_list::are_not_trivially_move_assignable())
        {
            _impl.set_value_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant& operator=(variant<that_value_types...>&& that)
            requires(value_types_list::are_moveable()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.set_value_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
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
        /// copy constructs variant with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(const value_type& value)
            requires(value_types_list::template has<value_type>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs variant with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(value_type&& value)
            requires(value_types_list::template has<value_type>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, move(value) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value and assigns new value.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant& operator=(const value_type& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.set_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// destroys previous value and assigns new value.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant& operator=(value_type&& value)
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
        /// emplaces the type `type` and returns it.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> value_type&
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            return _impl.template emplace_value<value_type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set(const value_type& value) -> void
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::is_copy_constructible())
        {
            _impl.set_value(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set(value_type&& value) -> void
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::is_move_constructible())
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return move(_impl.template get_value<value_type>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_checked() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_checked() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return move(_impl.template get_value<value_type>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_checked() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return move(_impl.template get_value<value_type>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the current value is of type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto is() const -> bool
            requires(value_types_list::template has<value_type>())
        {
            return _impl.template is_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the stored value is of any of the `that_value_types`.
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
            return _impl.get_type_index();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `that` holds the same type as `this` does and both values are equal
        /// as well.
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
