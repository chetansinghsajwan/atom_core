export module atom.core:core.option;

import :types;
import :contracts;
import :core.core;
import :core.option_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type used to initialize option with no value.
    /// --------------------------------------------------------------------------------------------
    export class nullopt
    {};

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
    export template <typename in_value_type>
    class option
    {
    private:
        using this_type = option<in_value_type>;
        using impl_type = option_impl<in_value_type>;
        using value_type_info = type_info<in_value_type>;

    public:
        static_assert(not value_type_info::is_void(), "value type must not be void.");
        static_assert(value_type_info::is_pure(), "value type must be pure.");
        static_assert(value_type_info::is_destructible(), "value type must be destructible.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value this option holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option()
            : _impl{ typename impl_type::ctor_default() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const option& that)
            requires(value_type_info::is_trivially_copy_constructible())
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        ///
        /// if `that` contains value, copy constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const option& that)
            requires(not value_type_info::is_trivially_copy_constructible()
                     and value_type_info::is_copy_constructible())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const option& that) -> option&
            requires(value_type_info::is_trivially_copy_assignable())
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
            requires(not value_type_info::is_trivially_copy_assignable()
                     and value_type_info::is_copyable())
        {
            _impl.copy(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option(option&& that)
            requires(value_type_info::is_trivially_move_constructible())
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        ///
        /// if `that` contains value, move constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(option&& that)
            requires(not value_type_info::is_trivially_move_constructible()
                     and value_type_info::is_move_constructible())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(option&& that) -> option&
            requires(value_type_info::is_trivially_move_assignable())
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
            requires(not value_type_info::is_trivially_move_assignable()
                     and value_type_info::is_moveable())
        {
            _impl.mov(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(nullopt)
            : _impl{ typename impl_type::ctor_default() }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator
        ///
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr option& operator=(nullopt)
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
            : _impl{ val }
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
            : _impl{ move(val) }
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
            requires(value_type_info::is_trivially_destructible())
        = default;

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        ///
        /// destroys value if stored.
        /// ----------------------------------------------------------------------------------------
        constexpr ~option()
            requires(not value_type_info::is_trivially_destructible()
                     and value_type_info::is_destructible())
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
            requires(value_type_info::template is_constructible_from<arg_types...>())
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
            contract_expects(is_value(), "does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() & -> value_type&
        {
            contract_expects(is_value(), "does not contain value.");

            return _impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ref.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() & -> value_type&
        {
            contract_expects(is_value(), "does not contain value.");

            return _impl.get_mut_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const value_type*
        {
            contract_debug_expects(is_value(), "does not contain value.");

            return &_impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// access the value by ptr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> value_type*
        {
            contract_debug_expects(is_value(), "does not contain value.");

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
        /// - `or_invoke`: function to return or_invoke value.
        ///
        /// # returns
        ///
        /// const ref to `this` value or or_invoke value returned by invoking `or_invoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        constexpr auto get_or_invoke(function_type&& or_invoke) const -> value_type
            requires(type_info<function_type>::template is_function<value_type()>())
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
        /// - `or_invoke`: function to return or_invoke value.
        ///
        /// # returns
        ///
        /// ref to `this` value or or_invoke value returned by invoking `or_invoke`.
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        constexpr auto get_mut_or_invoke(function_type&& or_invoke) -> value_type&
            requires(type_info<function_type>::template is_function<value_type&()>())
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
        constexpr auto get_or_default() const& -> value_type
            requires(value_type_info::is_default_constructible())
        {
            if (_impl.is_null())
            {
                return impl_type::get_default();
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
        constexpr auto operator==(nullopt) const -> bool
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
        constexpr auto operator==(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_equality_comparable_with<that_value_type>())
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
        constexpr auto operator<(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
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
        constexpr auto operator>(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
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
        constexpr auto operator<=(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
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
        constexpr auto operator>=(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
        {
            if (is_null() or that.is_null())
                return false;

            return get() >= that.get();
        }

    private:
        impl_type _impl;
    };
}
