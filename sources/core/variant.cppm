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
        using impl_type = _variant_impl<value_types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type_list of this variant.
        /// ----------------------------------------------------------------------------------------
        using value_types_list = type_list<value_types...>;

    public:
        static_assert(not value_types_list::is_empty(), "at least one type needs to be specified.");
        static_assert(value_types_list::are_pure(), "every type should be pure.");
        static_assert(value_types_list::are_unique(), "every type should be unique.");
        static_assert(value_types_list::are_destructible(), "every type should be destructible.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant()
            requires(typename value_types_list::first_type_info::is_default_constructible() or
                     typename value_types_list::first_type_info::is_void())
        {
            _impl.template construct_value_by_index<0>();
        }

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
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(const variant<that_value_types...>& that)
            requires(type_list<that_value_types...>::are_copy_constructible()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.construct_value_from_variant(that._impl);
        }

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
            _impl.set_value_from_variant(that._impl);
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
            _impl.set_value_from_variant(that._impl);
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
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... that_value_types>
        constexpr variant(variant<that_value_types...>&& that)
            requires(type_list<that_value_types...>::are_move_constructible()
                     and value_types_list::template has_all<that_value_types...>())
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

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
            _impl.set_value_from_variant(move(that._impl));
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
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs variant with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(const value_type& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.template construct_value_by_type<value_type>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs variant with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant(value_type&& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.template construct_value_by_type<value_type>(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value assigns new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to assign.
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
        /// destroys previous value assigns new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr variant& operator=(value_type&& value)
            requires(value_types_list::template has<value_type>())
        {
            _impl.set_value(move(value));
            return *this;
        }

        template <typename that_type>
        static consteval auto has_variant_constructor() -> bool
        {
            if constexpr (not type_info<that_type>::template is_derived_from<variant_tag>())
                return false;

            else if (type_info<this_type>::template is_same_as<that_type>())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr variant(create_from_variant_tag, const that_type& that)
            requires(has_variant_constructor<that_type>())
            : _impl{ create_from_variant, that._impl }
        {
            contract_asserts(get_index() != nums::get_max_usize());
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr variant(create_from_variant_tag, that_type&& that)
            requires(has_variant_constructor<that_type>())
            : _impl{ create_from_variant, move(that._impl) }
        {
            contract_asserts(get_index() != nums::get_max_usize());
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant()
            requires(not value_types_list::are_trivially_destructible())
        {
            _impl.destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// emplaces the type `type` and returns it.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> value_type&
            requires(value_types_list::template has<value_type>()
                     and type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            return _impl.template emplace_value_by_type<value_type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// emplaces the type for index `index` and returns it.
        /// ----------------------------------------------------------------------------------------
        template <usize index, typename... arg_types>
        constexpr auto emplace_at(arg_types&&... args)
            -> value_types_list::template at_type<index>& requires(
                value_types_list::is_index_in_range(index)
                and value_types_list::template at_type<index>::template is_constructible_from<
                    arg_types...>()) {
            _impl.template emplace_value_by_index<index>(forward<arg_types>(args)...);
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

            return _impl.template get_value_by_type<value_type>();
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

            return move(_impl.template get_value_by_type<value_type>());
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

            return _impl.template get_value_by_type<value_type>();
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

            return _impl.template get_value_by_type<value_type>();
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

            return move(_impl.template get_value_by_type<value_type>());
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

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() & -> value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() && -> value_type&&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return move(_impl.template get_value_by_type<value_type>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value as `value_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto get_unchecked() const -> const value_type&
            requires(value_types_list::template has<value_type>()
                     and not type_info<value_type>::is_void())
        {
            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() & -> value_types_list::template at_type<index>& requires(
                                      value_types_list::is_index_in_range(index)
                                      and not value_types_list::template at_type_info<
                                          index>::is_void()) {
            contract_debug_expects(is_index<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() && -> value_types_list::template at_type<index>&& requires(
                                       value_types_list::is_index_in_range(index)
                                       and not value_types_list::template at_type_info<
                                           index>::is_void()) {
            contract_debug_expects(is_index<index>(), "access to invalid type by index.");

            return move(_impl.template get_value_by_index<index>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() const
            -> type_utils::identity_type<const typename value_types_list::template at_type<index>&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            contract_expects(is_index<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() & -> type_utils::identity_type<
                                              typename value_types_list::template at_type<index>&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            contract_expects(is_index<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() && -> type_utils::identity_type<
                                               typename value_types_list::template at_type<index>&&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            contract_expects(is_index<index>(), "access to invalid type by index.");

            return move(_impl.template get_value_by_index<index>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() const
            -> type_utils::identity_type<const typename value_types_list::template at_type<index>&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            contract_expects(is_index<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_unchecked() & -> type_utils::identity_type<
                                                typename value_types_list::template at_type<index>&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_unchecked() && -> type_utils::identity_type<
                                                 typename value_types_list::template at_type<
                                                     index>&&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            return move(_impl.template get_value_by_index<index>());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the value accessed by index `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_unchecked() const
            -> type_utils::identity_type<const typename value_types_list::template at_type<index>&>
            requires(value_types_list::is_index_in_range(index)
                     and not value_types_list::template at_type_info<index>::is_void())
        {
            return _impl.template get_value_by_index<index>();
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
        /// returns `true` if the index of the current type is same as `index`.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto is_index() const -> bool
            requires(value_types_list::is_index_in_range(index))
        {
            return _impl.template is_index<i>();
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

    private:
        impl_type _impl;
    };
}
