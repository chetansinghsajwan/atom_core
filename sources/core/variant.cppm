export module atom.core:core.variant;

import :contracts;
import :types;
import :obj_helper;
import :core.core;
import :core.int_wrapper;
import :core.nums;
import :core.static_storage;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename... value_types>
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
        static_storage_for<value_types...> _storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// implementatiion of [`variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... value_types>
    class _variant_impl
    {
    private:
        template <typename... others_type>
        friend class _variant_impl;

    private:
        using _list_type = type_list<value_types...>;
        using _storage_type = _variant_storage<value_types...>;

    public:
        static constexpr auto get_type_count() -> usize
        {
            return _list_type::get_count();
        }

        template <typename value_type>
        static consteval auto has_type() -> bool
        {
            return _list_type::template has<value_type>();
        }

        template <usize i>
        static consteval auto has_index() -> bool
        {
            return i < _list_type::get_count();
        }

        template <typename value_type>
        static consteval auto get_index_for_type() -> usize
        {
            return _list_type::template get_index<value_type>();
        }

        static consteval auto get_null_type_index() -> usize
        {
            return nums::get_max<usize>();
        }

        template <usize i>
        using type_at_index = typename _list_type::template at_type<i>;

        using first_type = type_at_index<0>;

        using last_type = type_at_index<get_type_count() - 1>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copy or mov constructs value hold by variant `that`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr auto construct_value_from_variant(const _variant_impl<others_type...>& that)
        {
            _construct_value_from_variant_impl<false, 0, others_type...>(that, that.get_type_index());
        }

        template <typename... others_type>
        constexpr auto construct_value_from_variant(_variant_impl<others_type...>&& that)
        {
            _construct_value_from_variant_impl<true, 0, others_type...>(that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value from `that` variant.
        /// assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr auto set_value_from_variant(const _variant_impl<others_type...>& that)
        {
            _set_value_from_variant_impl<false, 0, others_type...>(that, that.get_type_index());
        }

        template <typename... others_type>
        constexpr auto set_value_from_variant(_variant_impl<others_type...>&& that)
        {
            _set_value_from_variant_impl<true, 0, others_type...>(that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto construct_value_by_type(arg_types&&... args)
        {
            _construct_value_as<value_type>(forward<arg_types>(args)...);
            _index = get_index_for_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_types>
        constexpr auto construct_value_by_index(arg_types&&... args)
        {
            construct_value_by_type<type_at_index<i>>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace_value_by_type(arg_types&&... args)
        {
            destroy_value();

            _construct_value_as<value_type>(forward<arg_types>(args)...);
            _index = get_index_for_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type at index `i` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <usize i, typename... arg_types>
        constexpr auto emplace_value_by_index(arg_types&&... args)
        {
            emplace_value_by_type<type_at_index<i>>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename in_value_type>
        constexpr auto set_value(in_value_type&& value)
        {
            using type = type_info<in_value_type>::pure_type::value_type;
            usize index_to_set = get_index_for_type<in_value_type>();

            // the new type to set is same as current.
            if (index_to_set == _index)
            {
                _assign_value_as<in_value_type>(forward<in_value_type>(value));
            }
            else
            {
                destroy_value();
                construct_value_by_type<in_value_type>(forward<in_value_type>(value));
            }
        }

        template <typename value_type>
        constexpr auto get_value_by_type() const -> const value_type&
        {
            contract_debug_expects(get_index_for_type<value_type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<value_type>();
        }

        template <typename value_type>
        constexpr auto get_value_by_type() -> value_type&
        {
            contract_debug_expects(get_index_for_type<value_type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<value_type>();
        }

        template <usize i>
        constexpr auto get_value_by_index() const -> const type_at_index<i>&
        {
            return get_value_by_type<type_at_index<i>>();
        }

        template <usize i>
        constexpr auto get_value_by_index() -> type_at_index<i>&
        {
            return get_value_by_type<type_at_index<i>>();
        }

        template <typename value_type>
        constexpr auto is_type() const -> bool
        {
            return get_index_for_type<value_type>() == get_type_index();
        }

        template <usize i>
        constexpr auto is_index() const -> bool
        {
            return i == get_type_index();
        }

        constexpr auto get_type_index() const -> usize
        {
            return _index;
        }

        constexpr auto destroy_value()
        {
            _destroy_value_impl<0, value_types...>(get_type_index());
        }

    private:
        template <bool mov, usize index, typename other_type, typename... others_type>
        constexpr auto _construct_value_from_variant_impl(auto& that, usize that_index)
        {
            using that_types = type_list<others_type...>;

            if (index != that_index)
            {
                if constexpr (that_types::get_count() == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    _construct_value_from_variant_impl<mov, index + 1, others_type...>(
                        that, that_index);
                    return;
                }
            }

            if constexpr (mov)
            {
                _construct_value_as<other_type>(move(that.template _get_value_as<other_type>()));
            }
            else
            {
                _construct_value_as<other_type>(that.template _get_value_as<other_type>());
            }

            _index = get_index_for_type<other_type>();
        }

        template <bool mov, usize index, typename other_type, typename... others_type>
        constexpr auto _set_value_from_variant_impl(auto&& that, usize that_index)
        {
            using that_types = type_list<others_type...>;

            if (index != that_index)
            {
                if constexpr (that_types::get_count() == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    _set_value_from_variant_impl<mov, index + 1, others_type...>(that, that_index);
                    return;
                }
            }

            // index for this variant of type same as that `variant` current type.
            usize index_for_this = get_index_for_type<other_type>();

            // we already have this value_type, so we don'type construct it but assign it.
            if (_index == index_for_this)
            {
                if constexpr (mov)
                {
                    _assign_value_as<other_type>(move(that.template _get_value_as<other_type>()));
                }
                else
                {
                    _assign_value_as<other_type>(that.template _get_value_as<other_type>());
                }
            }
            else
            {
                destroy_value();

                if constexpr (mov)
                {
                    _construct_value_as<other_type>(move(that.template _get_value_as<other_type>()));
                }
                else
                {
                    _construct_value_as<other_type>(that.template _get_value_as<other_type>());
                }

                _index = index_for_this;
            }
        }

        template <usize index, typename value_type, typename... others_type>
        constexpr auto _destroy_value_impl(usize i)
        {
            using _list_type = type_list<others_type...>;

            if (i != index)
            {
                if constexpr (_list_type::get_count() == 0)
                {
                    contract_panic("there is no type for current index.");
                }
                else
                {
                    // recursion to find type at index i.
                    _destroy_value_impl<index + 1, others_type...>(i);
                    return;
                }
            }

            _destruct_value_as<value_type>();
        }

        template <typename value_type, typename... arg_types>
        constexpr auto _construct_value_as(arg_types&&... args)
        {
            obj_helper().construct(_get_data_as<value_type>(), forward<arg_types>(args)...);
        }

        template <typename value_type, typename other_type>
        constexpr auto _assign_value_as(other_type&& val)
        {
            obj_helper().assign(_get_data_as<value_type>(), forward<other_type>(val));
        }

        template <typename value_type>
        constexpr auto _destruct_value_as()
        {
            if constexpr (type_info<value_type>::is_not_void)
                obj_helper().destruct(_get_data_as<value_type>());
        }

        template <typename value_type>
        constexpr auto _get_value_as() -> value_type&
        {
            return *_get_data_as<value_type>();
        }

        template <typename value_type>
        constexpr auto _get_value_as() const -> const value_type&
        {
            return *_get_data_as<value_type>();
        }

        template <typename value_type>
        constexpr auto _get_data_as() -> value_type*
        {
            return (value_type*)_storage.get_data();
        }

        template <typename value_type>
        constexpr auto _get_data_as() const -> const value_type*
        {
            return (const value_type*)_storage.get_data();
        }

    private:
        _storage_type _storage;
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
    /// - check if requirements using type_list functionality can be made concepts.
    /// --------------------------------------------------------------------------------------------
    export template <typename... value_types>
    class variant
    {
        static_assert(
            type_list<value_types...>::are_unique(), "every type in value_types... should be unique.");
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
            requires type_info<first_type>::is_default_constructible or type_info<first_type>::is_void
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
        template <typename... others_type>
        constexpr variant(const variant<others_type...>& that)
            requires type_list<others_type...>::info_type::are_copy_constructible
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
            requires value_types_list::are_copyable
                     and value_types_list::are_not_trivially_copy_assignable
        {
            _impl.set_value_from_variant(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant& operator=(const variant<others_type...>& that)
            requires value_types_list::are_copyable
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
            requires value_types_list::info_type::are_move_constructible
                     and value_types_list::info_type::are_not_trivially_move_constructible
        {
            _impl.construct_value_from_variant(move(that._impl));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant(variant<others_type...>&& that)
            requires type_list<others_type...>::info_type::are_move_constructible
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
            requires value_types_list::info_type::are_moveable
                     and value_types_list::info_type::are_not_trivially_move_assignable
        {
            _impl.set_value_from_variant(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment operator template
        /// ----------------------------------------------------------------------------------------
        template <typename... others_type>
        constexpr variant& operator=(variant<others_type...>&& that)
            requires value_types_list::info_type::are_moveable
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
            requires value_types_list::into_type::are_trivially_destructible
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
                    and (type_info<value_type>::template is_constructible_from<arg_types...>)
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
                    and (type_info<type_at<index>>::template is_constructible_from<arg_types...>)
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
            requires(has<value_type>()) and type_info<value_type>::is_not_void
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
            requires(has<value_type>()) and type_info<value_type>::is_not_void
        {
            contract_debug_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as_check() const -> const value_type&
            requires(has<value_type>()) and type_info<value_type>::is_not_void
        {
            contract_expects(is<value_type>(), "access to invalid type.");

            return _impl.template get_value_by_type<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        constexpr auto as_check() -> value_type&
            requires(has<value_type>()) and type_info<value_type>::is_not_void
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
            requires(has<index>()) and type_info<type_at<index>>::is_not_void
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
            requires(has<index>()) and type_info<type_at<index>>::is_not_void
        {
            contract_debug_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() const -> const type_at<index>&
            requires(has<index>()) and type_info<type_at<index>>::is_not_void
        {
            contract_expects(is<index>(), "access to invalid type by index.");

            return _impl.template get_value_by_index<index>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <usize index>
        constexpr auto get_at_checked() -> type_at<index>&
            requires(has<index>()) and type_info<type_at<index>>::is_not_void
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
