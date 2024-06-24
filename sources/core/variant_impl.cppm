export module atom.core:core.variant_impl;

import :contracts;
import :types;
import :obj_helper;
import :core.core;
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
            _construct_value_from_variant_impl<false, 0, others_type...>(
                that, that.get_type_index());
        }

        template <typename... others_type>
        constexpr auto construct_value_from_variant(_variant_impl<others_type...>&& that)
        {
            _construct_value_from_variant_impl<true, 0, others_type...>(
                that, that.get_type_index());
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

        template <typename... other_value_types>
        constexpr auto is_any_type() const -> bool
        {
            return type_list<other_value_types...>::is_any(
                [&](auto info) {
                    return get_index_for_type<typename decltype(info)::value_type>()
                           == get_type_index();
                });
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
                    _construct_value_as<other_type>(
                        move(that.template _get_value_as<other_type>()));
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
            if constexpr (not type_info<value_type>::is_void())
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
