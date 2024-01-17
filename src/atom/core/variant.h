#pragma once
#include "_variant_impl.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - check requirements for assignments.
    /// - check if requirements using type_list functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
        requires(type_list<ts...>::are_unique) and (type_list<ts...>::count > 0)
                and (not type_list<ts...>::template has<void>)
    class variant
    {
    private:
        using _impl_type = _variant_impl<ts...>;

        template <typename... tothers>
            requires(type_list<tothers...>::are_unique) and (type_list<tothers...>::count > 0)
                    and (not type_list<tothers...>::template has<void>)
        friend class variant;

        using self = variant<ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type_list of this variant.
        /// ----------------------------------------------------------------------------------------
        using types = type_list<ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// check if this variant supports type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        static consteval auto has() -> bool
        {
            return _impl_type::template has_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto has() -> bool
        {
            return _impl_type::template has_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
            requires(has<i>())
        using tat = typename _impl_type::template type_at_index<i>;

        /// ----------------------------------------------------------------------------------------
        /// index of type. this_type index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        static consteval auto index_of() -> usize
            requires(has<type>())
        {
            return _impl_type::template get_index_for_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of types this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto count() -> usize
        {
            return _impl_type::get_type_count();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant()
            requires(rdefault_constructible<tat<0>>)
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
            requires(rcopy_constructible_all<ts...>) and (not rtrivially_copy_constructible_all<ts...>)
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr variant(const variant<tothers...>& that)
            requires(types::template has<tothers...>) and (rcopy_constructible_all<tothers...>)
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
        constexpr auto operator=(const variant& that) -> variant&
            requires(rcopy_constructible_all<ts...>) and (rcopy_assignable_all<ts...>)
                    and (not rtrivially_copy_constructible_all<ts...>)
                    and (not rtrivially_copy_assignable_all<ts...>)
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr auto operator=(const variant<tothers...>& that) -> variant&
            requires(types::template has<tothers...>) and (rcopy_constructible_all<tothers...>)
                    and (rcopy_assignable_all<tothers...>)
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
            requires(rmove_constructible_all<ts...>) and (not rtrivially_move_constructible_all<ts...>)
        {
            _impl.construct_value_from_variant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr variant(variant<tothers...>&& that)
            requires(types::template has<tothers...>) and (rmove_constructible_all<tothers...>)
        {
            _impl.construct_value_from_variant(mov(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(variant&& that) -> variant& = default;

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(variant&& that) -> variant&
            requires(rmove_constructible_all<ts...>) and (rmove_assignable_all<ts...>)
                    and (not rtrivially_move_constructible_all<ts...>)
                    and (not rtrivially_move_assignable_all<ts...>)
        {
            _impl.set_value_from_variant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr auto operator=(variant<tothers...>&& that) -> variant&
            requires(types::template has<tothers...>) and (rmove_constructible_all<ts...>)
                    and (rmove_assignable_all<ts...>)
        {
            _impl.set_value_from_variant(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs variant with value of type type.
        ///
        /// # parameters
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant(const type& value)
            requires(has<type>())
        {
            _impl.template construct_value_by_type<type>(value);
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs variant with value of type type.
        ///
        /// # parameters
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr variant(type&& value)
            requires(has<type>())
        {
            _impl.template construct_value_by_type<type>(mov(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value assigns new value.
        ///
        /// # parameters
        /// - `value`: value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto operator=(const type& value) -> variant&
            requires(has<type>())
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
        /// - `value`: value to assign.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto operator=(type&& value) -> variant&
            requires(has<type>())
        {
            _impl.set_value(mov(value));
            return *this;
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
            requires(not rtrivially_destructible_all<ts...>)
        {
            _impl.destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructs the type `type` and sets the value.
        ///
        /// # see also
        /// - [`tat`]
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... args_type>
        constexpr auto emplace(args_type&&... args)
            requires(has<type>()) and (rconstructible<type, args_type...>)
        {
            _impl.template emplace_value_by_type<type>(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs the type for index `i` and sets the value.
        ///
        /// # see also
        /// - [`tat`]
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... args_type>
        constexpr auto emplace(args_type&&... args)
            requires(has<i>()) and (rconstructible<tat<i>, args_type...>)
        {
            _impl.template emplace_value_by_index<i>(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        /// - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename tfwd, typename type = tti::tremove_quailfiers_ref<tfwd>>
        constexpr auto set(tfwd&& value)
            requires(has<type>()) and (rconstructible<type, tfwd>)
        {
            _impl.set_value(forward<tfwd>(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        /// - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as() const -> const type&
            requires(has<type>())
        {
            contracts::expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        /// - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto as() -> type&
            requires(has<type>())
        {
            contracts::expects(is<type>(), "access to invalid type.");

            return _impl.template get_value_by_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `i`.
        ///
        /// # template parameters
        /// - `i`: index of type to access variants value as.
        ///
        /// # see also
        /// - [`tat`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto at() const -> const tat<i>&
            requires(has<i>())
        {
            contracts::expects(is<i>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value at index `i`.
        ///
        /// # template parameters
        /// - `i`: index of type to access variants value as.
        ///
        /// # see also
        /// - [`tat`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto at() -> tat<i>&
            requires(has<i>())
        {
            contracts::expects(is<i>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto is() const -> bool
            requires(has<type>())
        {
            return _impl.template is_type<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type accessed by index `i`.
        ///
        /// # see also
        /// - [`tat`]
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        constexpr auto is() const -> bool
            requires(has<i>())
        {
            return _impl.template is_index<i>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.get_type_index();
        }

    private:
        _impl_type _impl;
    };
}
