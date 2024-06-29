export module atom.core:core.variant_impl;

import :contracts;
import :types;
import :obj_helper;
import :core.core;
import :core.nums;
import :core.union_storage;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implementation of [`variant`].
    /// --------------------------------------------------------------------------------------------
    template <typename... value_types>
    class variant_impl
    {
        template <typename... that_value_types>
        friend class variant_impl;

    private:
        using this_type = variant_impl;
        using storage_type = union_storage<type_utils::empty_type, value_types...>;

    public:
        using value_types_list = type_list<value_types...>;

        struct that_tag
        {};

        template <typename value_type>
        struct emplace_tag
        {};

    public:
        constexpr variant_impl() = delete;

        constexpr variant_impl(const this_type& that) = default;
        constexpr variant_impl& operator=(const this_type& that) = default;

        constexpr variant_impl(this_type&& that) = default;
        constexpr variant_impl& operator=(this_type&& that) = default;

        constexpr variant_impl(that_tag, const this_type& that)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _index{ that._index }
        {
            value_types_list::for_each(
                [&](loop_command* command, auto info)
                {
                    using value_type = typename decltype(info)::value_type;

                    if (value_types_list::get_index(info) != _index)
                        return;

                    _construct_value_as<value_type>(that._get_value_as<value_type>());
                    *command = loop_command::break_;
                });
        }

        constexpr variant_impl(that_tag, this_type&& that)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _index{ that._index }
        {
            value_types_list::for_each(
                [&](loop_command* command, auto info)
                {
                    using value_type = typename decltype(info)::value_type;

                    if (value_types_list::get_index(info) != _index)
                        return;

                    _construct_value_as<value_type>(move(that._get_value_as<value_type>()));
                    *command = loop_command::break_;
                });
        }

        template <typename that_unpure_type>
        constexpr variant_impl(that_tag, that_unpure_type&& that)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _index{ nums::get_max_usize() }
        {
            using that_type = typename type_info<that_unpure_type>::pure_type::value_type;

            constexpr auto this_types_list = value_types_list{};
            constexpr auto that_types_list = typename that_type::value_types_list{};
            constexpr auto common_types_list = value_types_list::remove_others(that_types_list);

            constexpr bool should_move = type_info<decltype(that)>::is_rvalue_ref();

            usize that_current_index = that._index;

            common_types_list.for_each(
                [&](loop_command* command, auto type)
                {
                    using value_type = typename decltype(type)::value_type;
                    constexpr usize this_index = this_types_list.get_index(type);
                    constexpr usize that_index = that_types_list.get_index(type);

                    if (that_index != that_current_index)
                        return;

                    _index = this_index;

                    if (should_move)
                    {
                        _construct_value_as<value_type>(
                            move(that.template get_value<value_type>()));
                    }
                    else
                    {
                        _construct_value_as<value_type>(that.template get_value<value_type>());
                    }

                    *command = loop_command::break_;
                });

            if constexpr (this_types_list != common_types_list)
            {
                contract_asserts(_index != nums::get_max_usize());
            }
        }

        template <typename value_type, typename... arg_types>
        constexpr variant_impl(emplace_tag<value_type>, arg_types&&... args)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _index{ value_types_list::template get_index<value_type>() }
        {
            _construct_value_as<value_type>(forward<arg_types>(args)...);
        }

        constexpr variant_impl(emplace_tag<void>)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _index{ value_types_list::template get_index<void>() }
        {}

        constexpr ~variant_impl() = default;

        constexpr ~variant_impl()
            requires(not value_types_list::are_trivially_destructible())
        {
            _destroy_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value from `that`, assigns if `that` holds the same type else
        /// constructs.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        constexpr auto set_value_that(that_unpure_type&& that)
        {
            using that_type = typename type_info<that_unpure_type>::pure_type::value_type;
            using that_types_list = typename that_type::value_types_list;

            constexpr bool should_move = type_info<decltype(that)>::is_rvalue_ref();
            usize that_index = that._index;

            that_types_list::for_each(
                [&](loop_command* command, auto info)
                {
                    using value_type = typename decltype(info)::value_type;
                    if (that_types_list::get_index(info) != that_index)
                        return;

                    // index for this variant of type same as that `variant` current type.
                    constexpr usize this_index = value_types_list::get_index(info);

                    // we already have this value_type, so we don'type construct it but assign it.
                    if (_index == this_index)
                    {
                        if constexpr (should_move)
                        {
                            _assign_value_as<value_type>(
                                move(that.template _get_value_as<value_type>()));
                        }
                        else
                        {
                            _assign_value_as<value_type>(that.template _get_value_as<value_type>());
                        }
                    }
                    else
                    {
                        _destroy_value();

                        if constexpr (should_move)
                        {
                            _construct_value_as<value_type>(
                                move(that.template _get_value_as<value_type>()));
                        }
                        else
                        {
                            _construct_value_as<value_type>(
                                that.template _get_value_as<value_type>());
                        }

                        _index = this_index;
                    }

                    *command = loop_command::break_;
                });
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs value of type `type` with args `args`.
        ///
        /// # expects
        /// - current value is null.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr auto emplace_value(arg_types&&... args) -> value_type&
        {
            _destroy_value();

            _index = value_types_list::template get_index<value_type>();
            _construct_value_as<value_type>(forward<arg_types>(args)...);
            return _get_value_as<value_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// constructs or assigns value of type deduced by `value` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename other_value_type>
        constexpr auto set_value(other_value_type&& value)
        {
            usize other_index = value_types_list::template get_index<other_value_type>();

            // the new type to set is same as the current.
            if (_index == other_index)
            {
                _assign_value_as<other_value_type>(forward<other_value_type>(value));
            }
            else
            {
                _destroy_value();

                _index = other_index;
                _construct_value_as<other_value_type>(forward<other_value_type>(value));
            }
        }

        template <typename value_type>
        constexpr auto get_value() const -> const value_type&
        {
            return _get_value_as<value_type>();
        }

        template <typename value_type>
        constexpr auto get_value() -> value_type&
        {
            return _get_value_as<value_type>();
        }

        template <typename value_type>
        constexpr auto is_type() const -> bool
        {
            return _index == value_types_list::template get_index<value_type>();
        }

        template <typename... other_value_types>
        constexpr auto is_any_type() const -> bool
        {
            return type_list<other_value_types...>::is_any(
                [&](auto info) { return _index == value_types_list::get_index(info); });
        }

        constexpr auto get_index() const -> usize
        {
            return _index;
        }

        template <typename... that_value_types>
        constexpr auto is_eq(const variant_impl<that_value_types...>& that) const -> bool
        {
            using that_types_list = type_list<that_value_types...>;

            // they don't have the same type.
            if (value_types_list::get_id_at(_index) != that_types_list::get_id(that._index))
                return false;

            bool result;
            value_types_list::for_each(
                [&](loop_command* command, auto info)
                {
                    using value_type = typename decltype(info)::value_type;

                    if constexpr (value_types_list::get_index(info) != _index)
                        return;

                    result =
                        _get_value_as<value_type>() == that.template _get_value_as<value_type>();
                    *command = loop_command::break_;
                });

            return result;
        }

    private:
        constexpr auto _destroy_value()
        {
            value_types_list::for_each(
                [&](loop_command* command, auto info)
                {
                    if (value_types_list::get_index(info) != get_index())
                        return;

                    using value_type = typename decltype(info)::value_type;
                    _destruct_value_as<value_type>();
                    *command = loop_command::break_;
                });
        }

        template <typename value_type, typename... arg_types>
        constexpr auto _construct_value_as(arg_types&&... args)
        {
            obj_helper::construct(_get_data_as<value_type>(), forward<arg_types>(args)...);
        }

        template <typename value_type, typename other_type>
        constexpr auto _assign_value_as(other_type&& val)
        {
            obj_helper::assign(_get_data_as<value_type>(), forward<other_type>(val));
        }

        template <typename value_type>
        constexpr auto _destruct_value_as()
        {
            if constexpr (not type_info<value_type>::is_void())
            {
                obj_helper::destruct(_get_data_as<value_type>());
            }
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
            return _storage.template get_data_as<value_type>();
        }

        template <typename value_type>
        constexpr auto _get_data_as() const -> const value_type*
        {
            return _storage.template get_data_as<value_type>();
        }

    private:
        storage_type _storage;
        usize _index;
    };
}
