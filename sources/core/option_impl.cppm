export module atom.core:core.option_impl;

import :types;
import :obj_helper;
import :core.core;

namespace atom
{
    template <typename value_type>
    union option_storage
    {
        class _dummy
        {};

    public:
        constexpr option_storage()
            : _dummy{}
        {}

        constexpr option_storage(const option_storage&) = default;
        constexpr option_storage& operator=(const option_storage&) = default;

        constexpr option_storage(option_storage&&) = default;
        constexpr option_storage& operator=(option_storage&&) = default;

        template <typename... arg_types>
        constexpr option_storage(arg_types&&... args)
            : _value{ forward<arg_types>(args)... }
        {}

        constexpr ~option_storage()
            requires(type_info<value_type>::is_trivially_destructible())
        = default;

        constexpr ~option_storage()
            requires(not type_info<value_type>::is_trivially_destructible())
        {}

    public:
        constexpr auto get_data() -> value_type*
        {
            return &_value;
        }

        constexpr auto get_data() const -> const value_type*
        {
            return &_value;
        }

    private:
        value_type _value;
        _dummy _dummy;
    };

    template <typename in_value_type>
    class option_impl
    {
        using this_type = option_impl<in_value_type>;

        /// --------------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - create `static_aligned_storage_for<in_value_type>` to replace this.
        /// --------------------------------------------------------------------------------------------
        using _storage_type = option_storage<in_value_type>;

    public:
        using value_type = in_value_type;

        class ctor_default
        {};

        class that_tag
        {};

    public:
        /// ----------------------------------------------------------------------------------------
        /// get the default value of [`value_type`].
        /// ----------------------------------------------------------------------------------------
        static consteval auto get_default() -> value_type
        {
            return value_type();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # trivial default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl() = default;

        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(ctor_default)
            : _is_value{ false }
            , _storage{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(that_tag, const this_type& that)
            : this_type{ ctor_default() }
        {
            if (that._is_value)
            {
                _create_value(that._get_value());
                _is_value = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial mov constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # mov constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(that_tag, this_type&& that)
            : this_type{}
        {
            if (that._is_value)
            {
                _create_value(move(that._get_mut_value()));
                _is_value = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial mov operator
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl& operator=(this_type&& that) = default;

        template <typename... arg_types>
        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option_impl(arg_types&&... args)
            : _storage(forward<arg_types>(args)...)
            , _is_value{ true }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~option_impl() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copies [`option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy(const option_impl& that)
        {
            _set_value_from_option<false>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// movees [`option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mov(option_impl&& that)
        {
            _set_value_from_option<true>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// swaps [`option`] with `that`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(option_impl& that)
        {
            _swap(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroy current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            if (_is_value)
            {
                _destroy_value();
                _is_value = false;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any and constructs new value wih `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_value(arg_types&&... args)
        {
            if (_is_value)
            {
                _destroy_value();
                _create_value(forward<arg_types>(args)...);
            }
            else
            {
                _create_value(forward<arg_types>(args)...);
                _is_value = true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// if this contains value, assigns new value to it.
        /// else, constructs new value.
        /// ----------------------------------------------------------------------------------------
        template <typename type1>
        constexpr auto set_value(type1&& val)
        {
            if (not _is_value)
            {
                _create_value(forward<type1>(val));
                _is_value = true;
            }
            else
            {
                _set_value(forward<type1>(val));
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// get const ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_value() const -> const value_type&
        {
            return _get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get ref to current value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_value() -> value_type&
        {
            return _get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if this contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _is_value;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if this does not contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_null() const -> bool
        {
            return not _is_value;
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy_value()
        {
            if (_is_value)
            {
                _destroy_value();
                _is_value = false;
            }
        }

    private:
        template <bool mov, typename option_type>
        constexpr auto _set_value_from_option(option_type&& that)
        {
            if (that._is_value)
            {
                if (_is_value)
                {
                    if constexpr (mov)
                        _set_value(move(that._get_mut_value()));
                    else
                        _set_value(that._get_value());
                }
                else
                {
                    if constexpr (mov)
                        _create_value(move(that._get_mut_value()));
                    else
                        _create_value(that._get_value());

                    _is_value = true;
                }
            }
            else
            {
                if (_is_value)
                {
                    _destroy_value();
                    _is_value = false;
                }
            }
        }

        template <typename option_type>
        constexpr auto _copy(const option_type& that)
        {
            if (that._is_value)
            {
                if (_is_value)
                {
                    _set_value(that._get_value());
                }
                else
                {
                    _create_value(that._get_value());
                    _is_value = true;
                }
            }
            else
            {
                if (_is_value)
                {
                    _destroy_value();
                    _is_value = false;
                }
            }
        }

        template <typename option_type>
        constexpr auto _mov(option_type&& that)
        {
            if (that._is_value)
            {
                if (_is_value)
                {
                    _set_value(move(that._get_value()));
                }
                else
                {
                    _create_value(move(that._get_value()));
                    _is_value = true;
                }
            }
            else
            {
                if (_is_value)
                {
                    _destroy_value();
                    _is_value = false;
                }
            }
        }

        constexpr auto _swap(option_impl& that)
        {
            if (that._is_value)
            {
                if (_is_value)
                {
                    _swap_value(that._get_value());
                }
                else
                {
                    _create_value(move(that._get_mut_value()));
                    _is_value = true;
                    that._is_value = false;
                }
            }
            else
            {
                if (_is_value)
                {
                    that._create_value(move(_get_mut_value()));
                    that._is_value = true;
                    _is_value = false;
                }
            }
        }

        template <typename... arg_types>
        constexpr auto _create_value(arg_types&&... args)
        {
            obj_helper::construct_as<value_type>(_storage.get_data(), forward<arg_types>(args)...);
        }

        template <typename arg_type>
        constexpr auto _set_value(arg_type&& val)
        {
            obj_helper::assign_as<value_type>(_storage.get_data(), forward<arg_type>(val));
        }

        constexpr auto _swap_value(value_type& that)
        {
            obj_helper::swap(_get_mut_value(), that);
        }

        constexpr auto _get_value() const -> const value_type&
        {
            return *_storage.get_data();
        }

        constexpr auto _get_mut_value() -> value_type&
        {
            return *_storage.get_data();
        }

        constexpr auto _destroy_value()
        {
            obj_helper::destruct_as<value_type>(_storage.get_data());
        }

    private:
        bool _is_value;
        _storage_type _storage;
    };
}
