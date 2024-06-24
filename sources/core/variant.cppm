export module atom.core:core.variant;

import :contracts;
import :types;
import :core.core;
import :core.nums;
import :core.variant_impl;

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class variant_tag
    {};

    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - check requirements for assignments.
    /// - check if requirements using type_list functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    export template <typename... value_types>
    class variant: public variant_tag
    {
        static_assert(type_list<value_types...>::are_unique(),
            "every type in value_types... should be unique.");
        static_assert(
            type_list<value_types...>::get_count() > 0, "at least one type needs to be specified.");

    private:
        using this_type = variant<value_types...>;
        using impl_type = _variant_impl<value_types...>;

        template <typename... others_type>
        friend class variant;

        using self = variant<ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type_list of this variant.
        /// ----------------------------------------------------------------------------------------
        using value_types_list = type_list<value_types...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// check if this variant supports type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        static consteval auto has() -> bool
        {
            return impl_type::template has_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto has() -> bool
        {
            return impl_type::template has_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
            requires(has<index>())
        using type_at = typename impl_type::template type_at_index<index>;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using first_type = typename impl_type::first_type;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using last_type = typename impl_type::last_type;

        /// ----------------------------------------------------------------------------------------
        /// index of type. this index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        static consteval auto index_of() -> usize
            requires(has<value_type>())
        {
            return impl_type::template get_index_for_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of value_types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_count() -> usize
        {
            return impl_type::get_type_count();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant()
            requires(type_info<first_type>::is_default_constructible()
                     or type_info<first_type>::is_void())
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
            requires(value_types_list::info_list::are_copy_constructible())
                    and (value_types_list::info_list::are_not_trivially_copy_constructible())
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant(const variant<others_type...>& that)
            requires(type_list<others_type...>::are_copy_constructible())
                    and (value_types_list::template has<others_type...>())
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
            requires(value_types_list::are_copyable())
                    and (value_types_list::are_not_trivially_copy_assignable())
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant& operator=(const variant<others_type...>& that)
            requires(value_types_list::are_copyable())
                    and (value_types_list::template has<others_type...>())
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
            requires(value_types_list::are_move_constructible())
                    and (value_types_list::are_not_trivially_move_constructible())
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant(variant<others_type...>&& that)
            requires(type_list<others_type...>::are_move_constructible())
                    and (value_types_list::template has<others_type...>())
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
            requires(value_types_list::are_moveable())
                    and (value_types_list::are_not_trivially_move_assignable())
        {
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant& operator=(variant<others_type...>&& that)
            requires(value_types_list::are_moveable())
                    and (value_types_list::template has<others_type...>())
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
            requires(has<value_type>())
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
            requires(has<value_type>())
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
            requires(has<value_type>())
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
            requires(has<value_type>())
        {
            _impl.set_value(move(value));
            return *this;
        }

        template <typename that_type>
        static consteval auto is_variant_type() -> bool
        {
            return type_info<that_type>::template is_derived_from<variant_tag>();
        }

        template <typename that_type>
        static consteval auto should_enable_variant_constructor() -> bool
        {
            if constexpr (not is_variant_type<that_type>())
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
            requires(should_enable_variant_constructor<that_type>())
            : _impl{ create_from_variant, that._impl }
        {
            contract_asserts(index() != nums::get_max_usize());
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr variant(create_from_variant_tag, that_type&& that)
            requires(should_enable_variant_constructor<that_type>())
            : _impl{ create_from_variant, move(that._impl) }
        {
            contract_asserts(index() != nums::get_max_usize());
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant() = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        ///
        /// destructs value.
        /// ----------------------------------------------------------------------------------------
        constexpr ~variant()
            requires(value_types_list::into_type::are_trivially_destructible())
        {
            _impl.destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructs the type `type` and sets the value.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(has<value_type>())
                    and (type_info<value_type>::template is_constructible_from<arg_types...>())
        {
            _impl.template emplace_value_by_type<value_type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs the type for index `i` and sets the value.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index, typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(has<index>())
                    and (type_info<type_at<index>>::template is_constructible_from<arg_types...>())
        {
            _impl.template emplace_value_by_index<index>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set(const value_type&& value)
            requires(has<value_type>()) and type_info<value_type>::is_copy_constructible
        {
            _impl.set_value(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto set(value_type&& value)
            requires(has<value_type>()) and type_info<value_type>::is_move_constructible
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        ///
        ///  - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as() const -> const value_type&
            requires(has<value_type>()) and (not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        ///
        ///  - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as() -> value_type&
            requires(has<value_type>()) and (not type_info<value_type>::is_void())
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as_check() const -> const value_type&
            requires(has<value_type>()) and (not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as_check() -> value_type&
            requires(has<value_type>()) and (not type_info<value_type>::is_void())
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `index`.
        ///
        /// # template parameters
        ///
        ///  - `index`: index of type to access variants value as.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() const -> const type_at<index>&
            requires(has<index>()) and (not type_info<type_at<index>>::is_void())
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `i`.
        ///
        /// # template parameters
        ///
        ///  - `index`: index of type to access variants value as.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at() -> type_at<index>&
            requires(has<index>()) and (not type_info<type_at<index>>::is_void())
        {
            contract_debug_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() const -> const type_at<index>&
            requires(has<index>()) and (not type_info<type_at<index>>::is_void())
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() -> type_at<index>&
            requires(has<index>()) and (not type_info<type_at<index>>::is_void())
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto is() const -> bool
            requires(has<value_type>())
        {
            return _impl.template is_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type accessed by index `i`.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto is() const -> bool
            requires(has<i>())
        {
            return _impl.template is_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if the stored value is of any of the types `value_types`.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_value_types>
        constexpr auto is_any() const -> bool
            requires(value_types_list::template has_all<other_value_types...>())
        {
            return _impl.template is_any_type<other_value_types...>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.get_type_index();
        }

    private:
        impl_type _impl;
    };
}
