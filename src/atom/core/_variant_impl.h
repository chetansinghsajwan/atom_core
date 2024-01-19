#pragma once
#include "atom/core.h"
#include "atom/memory/obj_helper.h"
#include "atom/tti.h"
#include "atom/core/static_storage.h"
#include "atom/core/type_list.h"

namespace atom
{
    template <typename... ts>
    class _variant_storage
    {
    public:
        constexpr auto get_data() -> mut_mem_ptr<void>
        {
            return &_my_storage.storage;
        }

        constexpr auto get_data() const -> mem_ptr<void>
        {
            return &_my_storage.storage;
        }

    private:
        static_storage_for<ts...> _my_storage;
    };

    /// --------------------------------------------------------------------------------------------
    /// implementatiion of [`variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... ts>
    class _variant_impl
    {
    private:
        template <typename... tothers>
        friend class _variant_impl;

    private:
        using _types = type_list<ts...>;
        using _storage = _variant_storage<ts...>;

    public:
        static constexpr auto get_type_count() -> usize
        {
            return _types::count;
        }

        template <typename type>
        static consteval auto has_type() -> bool
        {
            return _types::template has<type>;
        }

        template <usize i>
        static consteval auto has_index() -> bool
        {
            return i < _type_list::count;
        }

        template <typename type>
        static consteval auto get_index_for_type() -> usize
        {
            return _types::template index_of<type>;
        }

        static consteval auto get_null_type_index() -> usize
        {
            return -1;
        }

        template <usize i>
        using type_at_index = typename _type_list::template at<i>;

        using first_type = type_at_index<0>;

        using last_type = type_at_index<get_type_count() - 1>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copy or move constructs value hold by variant `that`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr auto construct_value_from_variant(const _variant_impl<tothers...>& that)
        {
            _construct_value_from_variant_impl<false, 0, tothers...>(that, that.get_type_index());
        }

        template <typename... tothers>
        constexpr auto construct_value_from_variant(_variant_impl<tothers...>&& that)
        {
            _construct_value_from_variant_impl<true, 0, tothers...>(that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value from `that` variant.
        /// assigns if `that` variant holds the same type else constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename... tothers>
        constexpr auto set_value_from_variant(const _variant_impl<tothers...>& that)
        {
            _set_value_from_variant_impl<false, 0, tothers...>(that, that.get_type_index());
        }

        template <typename... tothers>
        constexpr auto set_value_from_variant(_variant_impl<tothers...>&& that)
        {
            _set_value_from_variant_impl<true, 0, tothers...>(that, that.get_type_index());
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto construct_value_by_type(arg_types&&... args)
        {
            _construct_value_as<type>(forward<arg_types>(args)...);
            _index = get_index_for_type<type>();
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
        template <typename type, typename... arg_types>
        constexpr auto emplace_value_by_type(arg_types&&... args)
        {
            destroy_value();

            _construct_value_as<type>(forward<arg_types>(args)...);
            _index = get_index_for_type<type>();
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
        template <typename value_type>
        constexpr auto set_value(value_type&& value)
        {
            using type = tti::remove_quailfiers_ref_type<value_type>;
            usize index_to_set = get_index_for_type<type>();

            // the new type to set is same as current.
            if (index_to_set == _index)
            {
                _assign_value_as<type>(forward<value_type>(value));
            }
            else
            {
                destroy_value();
                construct_value_by_type<type>(forward<value_type>(value));
            }
        }

        template <typename type>
        constexpr auto get_value_by_type() const -> const type&
        {
            contracts::debug_expects(get_index_for_type<type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<type>();
        }

        template <typename type>
        constexpr auto get_value_by_type() -> type&
        {
            contracts::debug_expects(get_index_for_type<type>() == get_type_index(),
                "current type is not same as requested type.");

            return _get_value_as<type>();
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

        template <typename type>
        constexpr auto is_type() const -> bool
        {
            return get_index_for_type<type>() == get_type_index();
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
            _destroy_value_impl<0, ts...>(get_type_index());
        }

    private:
        template <bool move, usize index, typename tother, typename... tothers>
        constexpr auto _construct_value_from_variant_impl(auto& that, usize that_index)
        {
            using that_types = type_list<tothers...>;

            if (index != that_index)
            {
                if constexpr (that_types::count == 0)
                {
                    system::panic("there is no type for current index.");
                }
                else
                {
                    _construct_value_from_variant_impl<move, index + 1, tothers...>(that, that_index);
                    return;
                }
            }

            if constexpr (move)
            {
                _construct_value_as<tother>(mov(that.template _get_value_as<tother>()));
            }
            else
            {
                _construct_value_as<tother>(that.template _get_value_as<tother>());
            }

            _index = get_index_for_type<tother>();
        }

        template <bool move, usize index, typename tother, typename... tothers>
        constexpr auto _set_value_from_variant_impl(auto&& that, usize that_index)
        {
            using that_types = type_list<tothers...>;

            if (index != that_index)
            {
                if constexpr (that_types::count == 0)
                {
                    system::panic("there is no type for current index.");
                }
                else
                {
                    _set_value_from_variant_impl<move, index + 1, tothers...>(that, that_index);
                    return;
                }
            }

            // index for this variant of type same as that `variant` current type.
            usize index_for_this = get_index_for_type<tother>();

            // we already have this type, so we don't construct it but assign it.
            if (_index == index_for_this)
            {
                if constexpr (move)
                {
                    _assign_value_as<tother>(mov(that.template _get_value_as<tother>()));
                }
                else
                {
                    _assign_value_as<tother>(that.template _get_value_as<tother>());
                }
            }
            else
            {
                destroy_value();

                if constexpr (move)
                {
                    _construct_value_as<tother>(mov(that.template _get_value_as<tother>()));
                }
                else
                {
                    _construct_value_as<tother>(that.template _get_value_as<tother>());
                }

                _index = index_for_this;
            }
        }

        template <usize index, typename type, typename... other_types>
        constexpr auto _destroy_value_impl(usize i)
        {
            using _type_list = type_list<other_types...>;

            if (i != index)
            {
                if constexpr (_type_list::count == 0)
                {
                    system::panic("there is no type for current index.");
                }
                else
                {
                    // recursion to find type at index i.
                    _destroy_value_impl<index + 1, other_types...>(i);
                    return;
                }
            }

            _destruct_value_as<type>();
        }

        template <typename type, typename... arg_types>
        constexpr auto _construct_value_as(arg_types&&... args)
        {
            obj_helper().construct(_get_data_as<type>(), forward<arg_types>(args)...);
        }

        template <typename type, typename t1>
        constexpr auto _assign_value_as(t1&& val)
        {
            obj_helper().assign(_get_data_as<type>(), forward<t1>(val));
        }

        template <typename type>
        constexpr auto _destruct_value_as()
        {
            obj_helper().destruct(_get_data_as<type>());
        }

        template <typename type>
        constexpr auto _get_value_as() -> type&
        {
            return _get_data_as<type>().get_mut();
        }

        template <typename type>
        constexpr auto _get_value_as() const -> const type&
        {
            return _get_data_as<type>().get();
        }

        template <typename type>
        constexpr auto _get_data_as() -> mut_mem_ptr<type>
        {
            return _storage.get_data();
        }

        template <typename type>
        constexpr auto _get_data_as() const -> mem_ptr<type>
        {
            return _storage.get_data();
        }

    private:
        _storage_type _storage;
        usize _index = 0;
    };
}
