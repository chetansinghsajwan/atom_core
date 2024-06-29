export module atom.core:core.option_impl;

import :types;
import :obj_helper;
import :core.core;
import :core.union_storage;

namespace atom
{
    template <typename in_value_type>
    class option_impl
    {
        using this_type = option_impl<in_value_type>;
        using value_type_info = type_info<in_value_type>;
        using storage_type = union_storage<in_value_type, type_utils::empty_type>;

    public:
        using value_type = in_value_type;

        class that_tag
        {};

        class null_tag
        {};

        class emplace_tag
        {};

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(that_tag, const this_type& that)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _is_value{ that._is_value }
        {
            if (that._is_value)
            {
                _construct_value(that._get_value());
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(that_tag, this_type&& that)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _is_value{ that._is_value }
        {
            if (that._is_value)
            {
                _construct_value(move(that._get_value()));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(null_tag)
            : _storage{ create_by_emplace<type_utils::empty_type> }
            , _is_value{ false }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr option_impl(emplace_tag, arg_types&&... args)
            : _storage{ create_by_emplace<value_type>, forward<arg_types>(args)... }
            , _is_value{ true }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~option_impl() = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~option_impl()
            requires(not value_type_info::is_trivially_destructible())
        {
            _destruct_value();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// copies or moves value from `that` into `this`.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        constexpr auto set_value_that(that_unpure_type&& that) -> void
        {
            constexpr bool should_move = type_info<decltype(that)>::is_rvalue_ref();

            if (that._is_value)
            {
                if (_is_value)
                {
                    if constexpr (should_move)
                        _assign_value(move(that._get_value()));
                    else
                        _assign_value(that._get_value());
                }
                else
                {
                    _is_value = true;

                    if constexpr (should_move)
                        _construct_value(move(that._get_value()));
                    else
                        _construct_value(that._get_value());
                }
            }
            else
            {
                if (_is_value)
                {
                    _is_value = false;
                    _destruct_value();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any and constructs new value wih `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_value(arg_types&&... args) -> void
        {
            if (_is_value)
            {
                _destruct_value();
                _construct_value(forward<arg_types>(args)...);
            }
            else
            {
                _is_value = true;
                _construct_value(forward<arg_types>(args)...);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// get ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_value() -> value_type&
        {
            return _get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get const ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_value() const -> const value_type&
        {
            return _get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset_value()
        {
            if (_is_value)
            {
                _is_value = false;
                _destruct_value();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if this contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _is_value;
        }

        /// --------------------------------------------------------------------------------------------
        /// # equality comparision
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_eq(const option_impl<that_value_type>& that) const -> bool
        {
            // one is null and one has value.
            if (_is_value != that._is_value)
                return false;

            // both are null.
            if (not _is_value)
                return true;

            return _get_value() == that._get_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than comparision
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_lt(const option_impl<that_value_type>& that) const -> bool
        {
            if (not _is_value or not that._is_value)
                return false;

            return _get_value() < that._get_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than comparision
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_gt(const option_impl<that_value_type>& that) const -> bool
        {
            if (not _is_value or not that._is_value)
                return false;

            return _get_value() > that._get_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than or equal to comparision
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_le(const option_impl<that_value_type>& that) const -> bool
        {
            if (not _is_value or not that._is_value)
                return false;

            return _get_value() <= that._get_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than or equal to comparision
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_ge(const option_impl<that_value_type>& that) const -> bool
        {
            if (not _is_value or not that._is_value)
                return false;

            return _get_value() >= that._get_value();
        }

    private:
        template <typename... arg_types>
        constexpr auto _construct_value(arg_types&&... args)
        {
            obj_helper::construct_as<value_type>(_get_data(), forward<arg_types>(args)...);
        }

        constexpr auto _destruct_value()
        {
            obj_helper::destruct_as<value_type>(_get_data());
        }

        template <typename arg_type>
        constexpr auto _assign_value(arg_type&& val)
        {
            obj_helper::assign_as<value_type>(_get_data(), forward<arg_type>(val));
        }

        constexpr auto _get_value() -> value_type&
        {
            return *_get_data();
        }

        constexpr auto _get_value() const -> const value_type&
        {
            return *_get_data();
        }

        constexpr auto _get_data() -> value_type*
        {
            return _storage.template get_data_as<value_type>();
        }

        constexpr auto _get_data() const -> const value_type*
        {
            return _storage.template get_data_as<value_type>();
        }

    private:
        bool _is_value;
        storage_type _storage;
    };
}
