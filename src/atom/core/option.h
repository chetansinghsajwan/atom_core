#pragma once
#include "atom/invokable/invokable.h"
#include "_option_impl.h"

namespace atom
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
        using impl = _option_impl<type>;

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
            : _impl(typename impl::ctor_default())
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
            requires(not rtrivially_copy_constructible<value_type>) and (rcopy_constructible<value_type>)
            : _impl(typename impl::ctor_copy(), that._impl)
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
            requires(not rtrivially_move_constructible<value_type>) and (rmove_constructible<value_type>)
            : _impl(typename impl::ctor_move(), mov(that._impl))
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
            _impl.move(mov(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(null_option)
            : _impl(typename impl::ctor_default())
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
            : _impl(mov(val))
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
            _impl.set_value(mov(val));
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
                return impl::get_default();
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
        constexpr auto eq(null_option) const -> bool
        {
            return is_null();
        }

        /// --------------------------------------------------------------------------------------------
        /// # not equality comparision
        ///
        /// `false` if this contains value, else `true`.
        /// --------------------------------------------------------------------------------------------
        constexpr auto ne(null_option) const -> bool
        {
            return is_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # equality comparision
        ///
        /// if `this` and `that` are null, returns `true`.
        /// if `this` is null and `that` is not null or vice versa, returns `false`.
        /// if `this` and `that` are not null, returns `this.get() == that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr auto eq(const option<tthat>& that) const -> bool
            requires(requality_comparable_with<value_type, tthat>)
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
        /// # not equality comparision
        ///
        /// performs negation of [equality comparision].
        /// --------------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr auto ne(const option<tthat>& that) const -> bool
            requires(requality_comparable_with<value_type, tthat>)
        {
            return not eq(that);
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than comparision
        ///
        /// if `this` or `that` is null, returns false.
        /// else, returns `this.get() < that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename tthat>
        constexpr auto lt(const option<tthat>& that) const -> bool
            requires(rcomparable_with<value_type, tthat>)
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
        template <typename tthat>
        constexpr auto gt(const option<tthat>& that) const -> bool
            requires(rcomparable_with<value_type, tthat>)
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
        template <typename tthat>
        constexpr auto le(const option<tthat>& that) const -> bool
            requires(rcomparable_with<value_type, tthat>)
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
        template <typename tthat>
        constexpr auto ge(const option<tthat>& that) const -> bool
            requires(rcomparable_with<value_type, tthat>)
        {
            if (is_null() or that.is_null())
                return false;

            return get() >= that.get();
        }

    private:
        impl _impl;
    };
}
