export module atom.core:option;
import :core;
import :tti;
import :invokable;
import :contracts_decl;
import :obj_helper;
import :mem_ptr;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename value_type>
    union _option_storage
    {
        class _dummy
        {};

    public:
        constexpr _option_storage()
            : _dummy()
        {}

        constexpr _option_storage(const _option_storage&) = default;
        constexpr _option_storage& operator=(const _option_storage&) = default;

        constexpr _option_storage(_option_storage&&) = default;
        constexpr _option_storage& operator=(_option_storage&&) = default;

        template <typename... arg_types>
        constexpr _option_storage(arg_types&&... args)
            : _value(forward<arg_types>(args)...)
        {}

        constexpr ~_option_storage()
            requires(rtrivially_destructible<value_type>)
        = default;

        constexpr ~_option_storage()
            requires(not rtrivially_destructible<value_type>)
        {}

    public:
        constexpr auto get_data() -> mut_mem_ptr<value_type>
        {
            return &_value;
        }

        constexpr auto get_data() const -> mem_ptr<value_type>
        {
            return &_value;
        }

    private:
        value_type _value;
        _dummy _dummy;
    };

    template <typename in_value_type>
    class _option_impl
    {
        using this_type = _option_impl<in_value_type>;

        /// --------------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - create `static_aligned_storage_for<in_value_type>` to replace this.
        /// --------------------------------------------------------------------------------------------
        using _storage_type = _option_storage<in_value_type>;

    public:
        using value_type = in_value_type;

        class ctor_default
        {};

        class ctor_copy
        {};

        class ctor_move
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
        constexpr _option_impl() = default;

        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(ctor_default)
            : _is_value(false)
            , _storage()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(ctor_copy, const this_type& that)
            : this_type(ctor_default())
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
        constexpr _option_impl& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # trivial mov constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # mov constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(ctor_move, this_type&& that)
            : this_type()
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
        constexpr _option_impl& operator=(this_type&& that) = default;

        template <typename... arg_types>
        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr _option_impl(arg_types&&... args)
            : _storage(forward<arg_types>(args)...)
            , _is_value(true)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~_option_impl() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copies [`option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto copy(const _option_impl& that)
        {
            _set_value_from_option<false>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// movees [`option`] into this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mov(_option_impl&& that)
        {
            _set_value_from_option<true>(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// swaps [`option`] with `that`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(_option_impl& that)
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

        constexpr auto _swap(_option_impl& that)
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
            obj_helper().construct_as<value_type>(_storage.get_data(), forward<arg_types>(args)...);
        }

        template <typename arg_type>
        constexpr auto _set_value(arg_type&& val)
        {
            obj_helper().assign_as<value_type>(_storage.get_data(), forward<arg_type>(val));
        }

        constexpr auto _swap_value(value_type& that)
        {
            obj_helper().swap(_get_mut_value(), that);
        }

        constexpr auto _get_value() const -> const value_type&
        {
            return _storage.get_data().get();
        }

        constexpr auto _get_mut_value() -> value_type&
        {
            return _storage.get_data().get_mut();
        }

        constexpr auto _destroy_value()
        {
            obj_helper().destruct_as<value_type>(_storage.get_data());
        }

    private:
        bool _is_value;
        _storage_type _storage;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type used to initialize option with no value.
    /// --------------------------------------------------------------------------------------------
    class null_option
    {};

    constexpr null_option nullopt = null_option();

    /// --------------------------------------------------------------------------------------------
    /// the option class is used to wrap the object of type `value_type`. this_type class contain either the
    /// value or can be empty representing no value.
    ///
    /// this_type is useful when we want to return a value that may or may not exist, without
    /// using null pointers or exceptions. or just want to add the ability of being null to a type
    /// like `i32`.
    ///
    /// # template parameters
    /// - `type`: type of value to store.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class option
    {
        static_assert(tti::is_pure<type>, "option supports only pure types");
        static_assert(not tti::is_void<type>, "option does not support void type.");

    private:
        using this_type = option<type>;
        using _impl_type = _option_impl<type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value this option holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option()
            : _impl(typename _impl_type::ctor_default())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const option& that)
            requires(rtrivially_copy_constructible<value_type>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        ///
        /// if `that` contains value, copy constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const option& that)
            requires(not rtrivially_copy_constructible<value_type>)
                    and (rcopy_constructible<value_type>)
            : _impl(typename _impl_type::ctor_copy(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const option& that) -> option&
            requires(rtrivially_copy_assignable<value_type>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        ///
        /// if `that` contains value
        ///     if `this` contains value, copy assigns with `that` value.
        ///     else, copy constructs with `that` value.
        /// else
        ///     if `this` contains value, destroys `this` value.
        ///     else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const option& that) -> option&
            requires(not rtrivially_copy_assignable<value_type>) and (rcopyable<value_type>)
        {
            _impl.copy(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option(option&& that)
            requires(rtrivially_move_constructible<value_type>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        ///
        /// if `that` contains value, move constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(option&& that)
            requires(not rtrivially_move_constructible<value_type>)
                    and (rmove_constructible<value_type>)
            : _impl(typename _impl_type::ctor_move(), move(that._impl))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(option&& that) -> option&
            requires(rtrivially_move_assignable<value_type>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        ///
        /// if `that` contains value
        ///     if `this` contains value, move assigns with `that` value.
        ///     else, move constructs with `that` value.
        /// else
        ///     if `this` contains value, destroys `this` value.
        ///     else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(option&& that) -> option&
            requires(not rtrivially_move_assignable<value_type>) and (rmoveable<value_type>)
        {
            _impl.mov(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(null_option)
            : _impl(typename _impl_type::ctor_default())
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator
        ///
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr option& operator=(null_option)
        {
            _impl.destroy_value();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs `this` value with `val`.
        ///
        /// # parameters
        ///
        /// - `val`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const value_type& val)
            : _impl(val)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// if `this` contains value, copy assigns `this` value with `val`.
        /// else, copy constructs `this` value with `val`.
        ///
        /// # parameters
        ///
        /// - `val`: value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const value_type& val) -> option&
        {
            _impl.set_value(val);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs `this` value with `value`.
        ///
        /// # parameters
        ///
        /// - `val`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr option(value_type&& val)
            : _impl(move(val))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// if `this` contains value, move assigns `this` value with `val`.
        /// else, move constructs `this` value with `val`.
        ///
        /// # parameters
        ///
        /// - `val`: value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(value_type&& val) -> option&
        {
            _impl.set_value(move(val));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~option()
            requires(rtrivially_destructible<value_type>)
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        ///
        /// destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr ~option()
            requires(not rtrivially_destructible<value_type>) and (rdestructible<value_type>)
        {
            _impl.destroy();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// construct value with `args`. if a value already exists, destroys that value and
        /// contructs and new one.
        ///
        /// # parameters
        ///
        /// - `args`: arguments to construct the new value with.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(rconstructible<value_type, arg_types...>)
        {
            _impl.emplace_value(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
        {
            return _impl.destroy_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const& -> const value_type&
        {
            contracts::expects(is_value(), "does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() & -> value_type&
        {
            contracts::expects(is_value(), "does not contain value.");

            return _impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const value_type*
        {
            contracts::debug_expects(is_value(), "does not contain value.");

            return &_impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> value_type*
        {
            contracts::debug_expects(is_value(), "does not contain value.");

            return &_impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get `this` value or `or_val`.
        ///
        /// if `this` contains value, get `this` value.
        /// else, get value `or_val`.
        ///
        /// # parameters
        ///
        /// - `or_val`: other value to return.
        ///
        /// # returns
        ///
        /// const ref to `this` value or `or_val`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_or(const value_type& or_val) const -> const value_type&
        {
            if (_impl.is_null())
            {
                return or_val;
            }

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get `this` value or `or_val`.
        ///
        /// if `this` contains value, get `this` value.
        /// else, get value `or_val`.
        ///
        /// # parameters
        ///
        /// - `or_val`: other value to return.
        ///
        /// # returns
        ///
        /// ref to `this` value or `or_val`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_or(value_type& or_val) -> value_type&
        {
            if (_impl.is_null())
            {
                return or_val;
            }

            return _impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get `this` value or.
        ///
        /// if `this` contains value, get `this` value.
        /// else, get value returned by invoking `or_invoke`.
        ///
        /// # parameters
        ///
        /// - `or_invoke`: invokable to return or_invoke value.
        ///
        /// # returns
        ///
        /// const ref to `this` value or or_invoke value returned by invoking `or_invoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        constexpr auto get_or_invoke(invokable_type&& or_invoke) const -> const value_type&
            requires rinvokable<pure_type<invokable_type>, const value_type&()>
        {
            if (_impl.is_null())
            {
                return or_invoke();
            }

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get `this` value or.
        ///
        /// if `this` contains value, get `this` value.
        /// else, get value returned by invoking `or_invoke`.
        ///
        /// # parameters
        ///
        /// - `or_invoke`: invokable to return or_invoke value.
        ///
        /// # returns
        ///
        /// ref to `this` value or or_invoke value returned by invoking `or_invoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_type>
        constexpr auto get_mut_or_invoke(invokable_type&& or_invoke) -> value_type&
            requires rinvokable<pure_type<invokable_type>, value_type&()>
        {
            if (_impl.is_null())
            {
                return or_invoke();
            }

            return _impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// get `this` value or default.
        ///
        /// if `this` contains value, get `this` value.
        /// else, get default constructed value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_or_default() const& -> const value_type&
            requires(rdefault_constructible<value_type>)
        {
            if (_impl.is_null())
            {
                return _impl_type::get_default();
            }

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _impl.is_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// is `this` contains value or not.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_null() const -> bool
        {
            return not _impl.is_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// swap values and state with `that`.
        ///
        /// if `that` contains value
        ///     if `this` contains value, swaps `this` value with `that` value.
        ///     else, move constructs `this` value with `that` value.
        /// else
        ///     if `this` contains value, move constructs `that` value with `this` value.
        ///     else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto swap(option& that)
        {
            return _impl.swap(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # equality comparision
        ///
        /// `true` if this contains value, else `false`.
        /// --------------------------------------------------------------------------------------------
        constexpr auto is_eq(null_option) const -> bool
        {
            return is_null();
        }

        /// --------------------------------------------------------------------------------------------
        /// # equality comparision
        ///
        /// if `this` and `that` are null, returns `true`.
        /// if `this` is null and `that` is not null or vice versa, returns `false`.
        /// if `this` and `that` are not null, returns `this.get() == that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_eq(const option<that_value_type>& that) const -> bool
            requires(requality_comparable_with<value_type, that_value_type>)
        {
            if (is_value() != that.is_value())
                // one is null and one has value.
                return false;

            if (is_null())
                // both are null.
                return true;

            return get() == that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than comparision
        ///
        /// if `this` or `that` is null, returns false.
        /// else, returns `this.get() < that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_lt(const option<that_value_type>& that) const -> bool
            requires(rcomparable_with<value_type, that_value_type>)
        {
            if (is_null() or that.is_null())
                return false;

            return get() < that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() > that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_gt(const option<that_value_type>& that) const -> bool
            requires(rcomparable_with<value_type, that_value_type>)
        {
            if (is_null() or that.is_null())
                return false;

            return get() > that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than or equal to comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() <= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_le(const option<that_value_type>& that) const -> bool
            requires(rcomparable_with<value_type, that_value_type>)
        {
            if (is_null() or that.is_null())
                return false;

            return get() <= that.get();
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than or equal to comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() >= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto is_ge(const option<that_value_type>& that) const -> bool
            requires(rcomparable_with<value_type, that_value_type>)
        {
            if (is_null() or that.is_null())
                return false;

            return get() >= that.get();
        }

    private:
        _impl_type _impl;
    };
}
