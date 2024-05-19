export module atom.core:core.variant;

import :contracts;
import :types;
import :math;
import :obj_helper;
import :core.core;
import :core.static_storage;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename... value_ts>
    class _variant_storage
    {
    public:
        constexpr auto get_data() -> void*
        {
            return &_storage.storage;
        }

        constexpr auto get_data() const -> const void*
        {
            return &_storage.storage;
        }

    private:
        static_storage_for<value_ts...> _storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// implementatiion of [`variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... value_ts>
    class _variant_impl
    {
    private:
        template <typename... others_t>
        friend class _variant_impl;

    private:
        using _list_t = typelist<value_ts...>;
        using _storage_t = _variant_storage<value_ts...>;

    public:
        static constexpr auto get_type_count() -> usize
        {
            return _list_t::count;
        }

        template <typename value_t>
        static consteval auto has_type() -> bool
        {
            return _list_t::template has<value_t>;
        }

        template <usize i>
        static consteval auto has_index() -> bool
        {
            return i < _list_t::count;
        }

        template <typename value_t>
        static consteval auto get_index_for_t() -> usize
        {
            return _list_t::template index_of<value_t>;
        }

        static consteval auto get_null_t_index() -> usize
        {
            return math::max<usize>();
        }

        template <usize i>
        using type_at_index = typename _list_t::template at<i>;

        using first_t = type_at_index<0>;

        using last_t = type_at_index<get_type_count() - 1>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copy or mov constructs value hold by variant `that`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr auto construct_value_from_variant(const _variant_impl<others_t...>& that)
        {
            _construct_value_from_variant_impl<false, 0, others_t...>(that, that.get_t_index());
        }

        template <typename... others_t>
        constexpr auto construct_value_from_variant(_variant_impl<others_t...>&& that)
        {
            _construct_value_from_variant_impl<true, 0, others_t...>(that, that.get_t_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value from `that` variant.
        /// assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr auto set_value_from_variant(const _variant_impl<others_t...>& that)
        {
            _set_value_from_variant_impl<false, 0, others_t...>(that, that.get_t_index());
        }

        template <typename... others_t>
        constexpr auto set_value_from_variant(_variant_impl<others_t...>&& that)
        {
            _set_value_from_variant_impl<true, 0, others_t...>(that, that.get_t_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t, typename... arg_ts>
        constexpr auto construct_value_by_t(arg_ts&&... args)
        {
            _construct_value_as<value_t>(forward<arg_ts>(args)...);
            _index = get_index_for_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_ts>
        constexpr auto construct_value_by_index(arg_ts&&... args)
        {
            construct_value_by_t<type_at_index<i>>(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t, typename... arg_ts>
        constexpr auto emplace_value_by_t(arg_ts&&... args)
        {
            destroy_value();

            _construct_value_as<value_t>(forward<arg_ts>(args)...);
            _index = get_index_for_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_ts>
        constexpr auto emplace_value_by_index(arg_ts&&... args)
        {
            emplace_value_by_t<type_at_index<i>>(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename in_value_t>
        constexpr auto set_value(in_value_t&& value)
        {
            using type = typeinfo<in_value_t>::pure_t::value_t;
            usize index_to_set = get_index_for_t<in_value_t>();

            // the new type to set is same as current.
            if (index_to_set == _index)
            {
                _assign_value_as<in_value_t>(forward<in_value_t>(value));
            }
            else
            {
                destroy_value();
                construct_value_by_t<in_value_t>(forward<in_value_t>(value));
            }
        }

        template <typename value_t>
        constexpr auto get_value_by_t() const -> const value_t&
        {
            contract_debug_expects(get_index_for_t<value_t>() == get_t_index(),
                "current type is not same as requested type.");

            return _get_value_as<value_t>();
        }

        template <typename value_t>
        constexpr auto get_value_by_t() -> value_t&
        {
            contract_debug_expects(get_index_for_t<value_t>() == get_t_index(),
                "current type is not same as requested type.");

            return _get_value_as<value_t>();
        }

        template <usize i>
        constexpr auto get_value_by_index() const -> const type_at_index<i>&
        {
            return get_value_by_t<type_at_index<i>>();
        }

        template <usize i>
        constexpr auto get_value_by_index() -> type_at_index<i>&
        {
            return get_value_by_t<type_at_index<i>>();
        }

        template <typename value_t>
        constexpr auto is_t() const -> bool
        {
            return get_index_for_t<value_t>() == get_t_index();
        }

        template <usize i>
        constexpr auto is_index() const -> bool
        {
            return i == get_t_index();
        }

        constexpr auto get_t_index() const -> usize
        {
            return _index;
        }

        constexpr auto destroy_value()
        {
            _destroy_value_impl<0, value_ts...>(get_t_index());
        }

    private:
        template <bool mov, usize index, typename other_t, typename... others_t>
        constexpr auto _construct_value_from_variant_impl(auto& that, usize that_index)
        {
            using that_ts = typelist<others_t...>;

            if (index != that_index)
            {
                if constexpr (that_ts::count == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    _construct_value_from_variant_impl<mov, index + 1, others_t...>(
                        that, that_index);
                    return;
                }
            }

            if constexpr (mov)
            {
                _construct_value_as<other_t>(move(that.template _get_value_as<other_t>()));
            }
            else
            {
                _construct_value_as<other_t>(that.template _get_value_as<other_t>());
            }

            _index = get_index_for_t<other_t>();
        }

        template <bool mov, usize index, typename other_t, typename... others_t>
        constexpr auto _set_value_from_variant_impl(auto&& that, usize that_index)
        {
            using that_ts = typelist<others_t...>;

            if (index != that_index)
            {
                if constexpr (that_ts::count == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    _set_value_from_variant_impl<mov, index + 1, others_t...>(that, that_index);
                    return;
                }
            }

            // index for this variant of type same as that `variant` current type.
            usize index_for_this = get_index_for_t<other_t>();

            // we already have this value_t, so we don'type construct it but assign it.
            if (_index == index_for_this)
            {
                if constexpr (mov)
                {
                    _assign_value_as<other_t>(move(that.template _get_value_as<other_t>()));
                }
                else
                {
                    _assign_value_as<other_t>(that.template _get_value_as<other_t>());
                }
            }
            else
            {
                destroy_value();

                if constexpr (mov)
                {
                    _construct_value_as<other_t>(move(that.template _get_value_as<other_t>()));
                }
                else
                {
                    _construct_value_as<other_t>(that.template _get_value_as<other_t>());
                }

                _index = index_for_this;
            }
        }

        template <usize index, typename value_t, typename... others_t>
        constexpr auto _destroy_value_impl(usize i)
        {
            using _list_t = typelist<others_t...>;

            if (i != index)
            {
                if constexpr (_list_t::count == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    // recursion to find type at index i.
                    _destroy_value_impl<index + 1, others_t...>(i);
                    return;
                }
            }

            _destruct_value_as<value_t>();
        }

        template <typename value_t, typename... arg_ts>
        constexpr auto _construct_value_as(arg_ts&&... args)
        {
            obj_helper().construct(_get_data_as<value_t>(), forward<arg_ts>(args)...);
        }

        template <typename value_t, typename other_t>
        constexpr auto _assign_value_as(other_t&& val)
        {
            obj_helper().assign(_get_data_as<value_t>(), forward<other_t>(val));
        }

        template <typename value_t>
        constexpr auto _destruct_value_as()
        {
            if constexpr (typeinfo<value_t>::is_not_void)
                obj_helper().destruct(_get_data_as<value_t>());
        }

        template <typename value_t>
        constexpr auto _get_value_as() -> value_t&
        {
            return *_get_data_as<value_t>();
        }

        template <typename value_t>
        constexpr auto _get_value_as() const -> const value_t&
        {
            return *_get_data_as<value_t>();
        }

        template <typename value_t>
        constexpr auto _get_data_as() -> value_t*
        {
            return (value_t*)_storage.get_data();
        }

        template <typename value_t>
        constexpr auto _get_data_as() const -> const value_t*
        {
            return (const value_t*)_storage.get_data();
        }

    private:
        _storage_t _storage;
        usize _index = 0;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// # to do
    /// - check requirements for assignments.
    /// - check if requirements using typelist functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    export template <typename... value_ts>
    class variant
    {
        static_assert(
            typelist<value_ts...>::are_unique(), "every type in value_ts... should be unique.");
        static_assert(
            typelist<value_ts...>::count > 0, "at least one type needs to be specified.");

    private:
        using this_t = variant<value_ts...>;
        using impl_t = _variant_impl<value_ts...>;

        template <typename... others_t>
        friend class variant;

        using self = variant<ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// typelist of this variant.
        /// ----------------------------------------------------------------------------------------
        using value_types_list = typelist<value_ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// check if this variant supports type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        static consteval auto has() -> bool
        {
            return impl_t::template has_type<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// check if index `i` can be used to access value.
        /// ----------------------------------------------------------------------------------------
        template <usize i>
        static consteval auto has() -> bool
        {
            return impl_t::template has_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get type at index.
        /// ----------------------------------------------------------------------------------------
        template <usize index>
            requires(has<index>())
        using type_at = typename impl_t::template type_at_index<index>;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using first_t = typename impl_t::first_t;

        /// ----------------------------------------------------------------------------------------
        /// get first type.
        /// ----------------------------------------------------------------------------------------
        using last_t = typename impl_t::last_t;

        /// ----------------------------------------------------------------------------------------
        /// index of type. this index than can be used to access value of type at that index.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        static consteval auto index_of() -> usize
            requires(has<value_t>())
        {
            return impl_t::template get_index_for_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// count of value_ts this variant supports.
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_count() -> usize
        {
            return impl_t::get_type_count();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr variant()
            requires typeinfo<first_t>::is_default_constructible or typeinfo<first_t>::is_void
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
            requires value_types_list::info_list::are_copy_constructible
                     and value_types_list::info_list::are_not_trivially_copy_constructible
        {
            _impl.construct_value_from_variant(that._impl);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr variant(const variant<others_t...>& that)
            requires typelist<others_t...>::info_t::are_copy_constructible
                     and (value_types_list::template has<others_t...>)
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
            requires value_types_list::are_copyable
                     and value_types_list::are_not_trivially_copy_assignable
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr variant& operator=(const variant<others_t...>& that)
            requires value_types_list::are_copyable
                     and (value_types_list::template has<others_t...>)
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
            requires value_types_list::info_t::are_move_constructible
                     and value_types_list::info_t::are_not_trivially_move_constructible
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr variant(variant<others_t...>&& that)
            requires typelist<others_t...>::info_t::are_move_constructible
                     and (value_types_list::template has<others_t...>)
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
            requires value_types_list::info_t::are_moveable
                     and value_types_list::info_t::are_not_trivially_move_assignable
        {
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_t>
        constexpr variant& operator=(variant<others_t...>&& that)
            requires value_types_list::info_t::are_moveable
                     and (value_types_list::template has<others_t...>)
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
        template <typename value_t>
        constexpr variant(const value_t& value)
            requires(has<value_t>())
        {
            _impl.template construct_value_by_t<value_t>(value);
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
        template <typename value_t>
        constexpr variant(value_t&& value)
            requires(has<value_t>())
        {
            _impl.template construct_value_by_t<value_t>(move(value));
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
        template <typename value_t>
        constexpr variant& operator=(const value_t& value)
            requires(has<value_t>())
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
        template <typename value_t>
        constexpr variant& operator=(value_t&& value)
            requires(has<value_t>())
        {
            _impl.set_value(move(value));
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
            requires value_types_list::into_t::are_trivially_destructible
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
        template <typename value_t, typename... arg_ts>
        constexpr auto emplace(arg_ts&&... args)
            requires(has<value_t>())
                    and (typeinfo<value_t>::template is_constructible_from<arg_ts...>)
        {
            _impl.template emplace_value_by_t<value_t>(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs the type for index `i` and sets the value.
        ///
        /// # see also
        ///
        /// - [`type_at`]
        /// ----------------------------------------------------------------------------------------
        template <usize index, typename... arg_ts>
        constexpr auto emplace(arg_ts&&... args)
            requires(has<index>())
                    and (typeinfo<type_at<index>>::template is_constructible_from<arg_ts...>)
        {
            _impl.template emplace_value_by_index<index>(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// sets the value to `value`.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        constexpr auto set(const value_t&& value)
            requires(has<value_t>()) and typeinfo<value_t>::is_copy_constructible
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
        template <typename value_t>
        constexpr auto set(value_t&& value)
            requires(has<value_t>()) and typeinfo<value_t>::is_move_constructible
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
        template <typename value_t>
        constexpr auto as() const -> const value_t&
            requires(has<value_t>()) and typeinfo<value_t>::is_not_void
        {
            contract_expects(is<value_t>(), "access to invalid type.");

            return _impl.template get_value_by_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value as type `type`.
        ///
        /// # template parameters
        ///
        ///  - `type`: type to access variant's value as.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        constexpr auto as() -> value_t&
            requires(has<value_t>()) and typeinfo<value_t>::is_not_void
        {
            contract_debug_expects(is<value_t>(), "access to invalid type.");

            return _impl.template get_value_by_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        constexpr auto as_check() const -> const value_t&
            requires(has<value_t>()) and typeinfo<value_t>::is_not_void
        {
            contract_expects(is<value_t>(), "access to invalid type.");

            return _impl.template get_value_by_t<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        constexpr auto as_check() -> value_t&
            requires(has<value_t>()) and typeinfo<value_t>::is_not_void
        {
            contract_expects(is<value_t>(), "access to invalid type.");

            return _impl.template get_value_by_t<value_t>();
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
            requires(has<index>()) and typeinfo<type_at<index>>::is_not_void
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
            requires(has<index>()) and typeinfo<type_at<index>>::is_not_void
        {
            contract_debug_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at_check() const -> const type_at<index>&
            requires(has<index>()) and typeinfo<type_at<index>>::is_not_void
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto at_check() -> type_at<index>&
            requires(has<index>()) and typeinfo<type_at<index>>::is_not_void
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if current value is of type `type`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        constexpr auto is() const -> bool
            requires(has<value_t>())
        {
            return _impl.template is_t<value_t>();
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
        /// get the index to current type.
        /// ----------------------------------------------------------------------------------------
        constexpr auto index() const -> usize
        {
            return _impl.get_t_index();
        }

    private:
        impl_t _impl;
    };
}
