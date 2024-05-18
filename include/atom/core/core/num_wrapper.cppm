export module atom.core:core.num_wrapper;

import std;
import :contracts;

namespace atom
{
    class _num_wrapper_id
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_t>
    concept _is_num = std::is_integral_v<num_t> or std::is_floating_point_v<num_t>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_t>
    concept is_num = std::derived_from<num_t, _num_wrapper_id>;

    /// --------------------------------------------------------------------------------------------
    /// wraps any numeric type to provide safe operations like overflow and underflow checks.
    /// --------------------------------------------------------------------------------------------
    template <typename in_impl_t>
    class num_wrapper: public _num_wrapper_id
    {
        using this_t = num_wrapper<in_impl_t>;

    protected:
        using impl_t = in_impl_t;
        using final_t = typename impl_t::final_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type that `this_t` wraps.
        /// ----------------------------------------------------------------------------------------
        using unwrapped_t = typename impl_t::unwrapped_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper() = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy contructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper(const this_t&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper& operator=(const this_t&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move contructor
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper(this_t&&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper& operator=(this_t&&) = default;

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        ///
        /// converts `num_t` to `this_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        explicit constexpr num_wrapper(num_t num)
            requires is_num<num_t>
            : _value(num._value)
        {
            contract_debug_expects(is_conversion_safe_from(num));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        ///
        /// converts `num_t` to `this_t`. this is supposed to accept literals only.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr num_wrapper(num_t num)
            requires _is_num<num_t>
            : _value(num)
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));
        }

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        ///
        /// converts `num_t` to `this_t`. this is supposed to accept literals only.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto operator=(num_t num) -> final_t&
            requires _is_num<num_t>
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));

            _value = num;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~num_wrapper() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is an integer type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_integer() -> bool
        {
            return impl_t::is_integer();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is an integer type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_float() -> bool
        {
            return impl_t::is_float();
        }

        /// ----------------------------------------------------------------------------------------
        /// counts number of digits needed to represent `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_digits() const -> final_t
        {
            return _wrap_final(impl_t::count_digits(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is a signed type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_signed() -> bool
        {
            return impl_t::is_signed();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto nan() -> this_t
            requires(is_float())
        {
            return _wrap_final(impl_t::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> this_t
            requires(is_float())
        {
            return _wrap_final(impl_t::floor(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> this_t
            requires(is_float())
        {
            return _wrap_final(impl_t::ceil(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> this_t
            requires(is_float())
        {
            return _wrap_final(impl_t::round(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from(num_t num) -> final_t
            requires is_num<num_t>
        {
            contract_debug_expects(is_conversion_safe_from(num));

            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from_checked(num_t num) -> final_t
            requires is_num<num_t>
        {
            contract_expects(is_conversion_safe_from(num));

            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from_unchecked(num_t num) -> final_t
            requires is_num<num_t>
        {
            return _wrap_final(num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when converting `num_t` to
        /// `this_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto is_conversion_safe_from(num_t num) -> bool
            requires is_num<num_t>
        {
            return impl_t::is_conversion_safe_from(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from_unwrapped(num_t num) -> final_t
            requires _is_num<num_t>
        {
            contract_debug_expects(is_conversion_safe_from_unwrapped(num));

            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from_unwrapped_checked(num_t num) -> final_t
            requires _is_num<num_t>
        {
            contract_expects(is_conversion_safe_from_unwrapped(num));

            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// creates `this_t` from unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto from_unwrapped_unchecked(num_t num) -> final_t
            requires _is_num<num_t>
        {
            return _wrap_final(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when creating `this_t` from
        /// `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        static constexpr auto is_conversion_safe_from_unwrapped(num_t num) -> bool
            requires _is_num<num_t>
        {
            return impl_t::is_conversion_safe_from_unwrapped(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_t::from(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to() const -> num_t
            requires is_num<num_t>
        {
            return num_t::from(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_t::from_checked(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to_checked() const -> num_t
            requires is_num<num_t>
        {
            return num_t::from_checked(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num_t::from_unchecked(*this)`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to_unchecked() const -> num_t
            requires is_num<num_t>
        {
            return num_t::from_unchecked(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when creating `this_t` from
        /// `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto is_conversion_safe_to() -> bool
            requires is_num<num_t>
        {
            return num_t::template is_conversion_safe_from<this_t>(_this_final());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `unwrapped_t` value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_unwrapped() const -> unwrapped_t
        {
            return _value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to_unwrapped() const -> num_t
            requires _is_num<num_t>
        {
            contract_debug_expects(is_conversion_safe_to_unwrapped<num_t>());

            return num_t(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to_unwrapped_checked() const -> num_t
            requires _is_num<num_t>
        {
            contract_expects(is_conversion_safe_to_unwrapped<num_t>());

            return num_t(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` converted to unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto to_unwrapped_unchecked() const -> num_t
            requires _is_num<num_t>
        {
            return num_t(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when converting `this_t` to
        /// unwrapped `num_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename num_t>
        constexpr auto is_conversion_safe_to_unwrapped() const -> bool
            requires _is_num<num_t>
        {
            return impl_t::template is_conversion_safe_to_unwrapped<num_t>(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add(final_t num) const -> final_t
        {
            contract_debug_expects(is_add_safe(num));

            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_checked(final_t num) const -> final_t
        {
            contract_expects(is_add_safe(num));

            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_unchecked(final_t num) const -> final_t
        {
            return _wrap_final(_value + num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+(final_t num) const -> final_t
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign(final_t num) -> final_t&
        {
            contract_debug_expects(is_add_safe(num));

            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_checked(final_t num) -> final_t&
        {
            contract_expects(is_add_safe(num));

            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after adding `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_unchecked(final_t num) -> final_t&
        {
            _value += num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+=(final_t num) -> final_t&
        {
            contract_debug_expects(is_add_safe(num));

            return add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`add_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> final_t&
        {
            return add_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during addition.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_add_safe(final_t num) const -> bool
        {
            return impl_t::is_add_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(final_t num) const -> final_t
        {
            contract_debug_expects(is_sub_safe(num));

            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_checked(final_t num) const -> final_t
        {
            contract_expects(is_sub_safe(num));

            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_unchecked(final_t num) const -> final_t
        {
            return _wrap_final(_value - num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-(final_t num) const -> final_t
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign(final_t num) -> final_t&
        {
            contract_debug_expects(is_sub_safe(num));

            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_checked(final_t num) -> final_t&
        {
            contract_expects(is_sub_safe(num));

            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after subtracting `num` from `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_unchecked(final_t num) -> final_t&
        {
            _value -= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-=(final_t num) -> final_t&
        {
            return sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`sub_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> final_t&
        {
            return sub_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during subtraction.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_sub_safe(final_t num) const -> bool
        {
            return impl_t::is_sub_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul(final_t num) const -> final_t
        {
            contract_debug_expects(is_mul_safe(num));

            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_checked(final_t num) const -> final_t
        {
            contract_expects(is_mul_safe(num));

            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_unchecked(final_t num) const -> final_t
        {
            return _wrap_final(_value * num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*(final_t num) const -> final_t
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign(final_t num) -> final_t&
        {
            contract_debug_expects(is_mul_safe(num));

            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_checked(final_t num) -> final_t&
        {
            contract_expects(is_mul_safe(num));

            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores result after multiplying `this` with `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_unchecked(final_t num) -> final_t&
        {
            _value *= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`mul_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*=(final_t num) -> final_t&
        {
            return mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during multiplication.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_mul_safe(final_t num) const -> bool
        {
            return impl_t::is_mul_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div(final_t num) const -> final_t
        {
            contract_debug_expects(is_div_safe(num));

            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_checked(final_t num) const -> final_t
        {
            contract_expects(is_div_safe(num));

            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_unchecked(final_t num) const -> final_t
        {
            return _wrap_final(_value / num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/(final_t num) const -> final_t
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign(final_t num) -> final_t&
        {
            contract_debug_expects(is_div_safe(num));

            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_checked(final_t num) -> final_t&
        {
            contract_expects(is_div_safe(num));

            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores quotient after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_unchecked(final_t num) -> final_t&
        {
            _value /= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`div_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/=(final_t num) -> final_t&
        {
            return div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem(final_t num) const -> final_t
        {
            contract_debug_expects(is_div_safe(num));

            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_checked(final_t num) const -> final_t
        {
            contract_expects(is_div_safe(num));

            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_unchecked(final_t num) const -> final_t
        {
            return _wrap_final(_value % num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`rem(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator%(final_t num) const -> final_t
        {
            return rem(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign(final_t num) -> final_t&
        {
            contract_debug_expects(is_div_safe(num));

            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign_checked(final_t num) -> final_t&
        {
            contract_expects(is_div_safe(num));

            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// stores remainder after dividing `this` by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rem_assign_unchecked(final_t num) -> final_t&
        {
            _value %= num._value;
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`rem_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator%=(final_t num) -> final_t&
        {
            return rem_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is no overflow or underflow occurs during division.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_div_safe(final_t num) const -> bool
        {
            return impl_t::is_div_safe(_value, num._value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns minimum value of `this_t`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto min() -> final_t
        {
            return _wrap_final(impl_t::min());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns maximum value of `this_t`.
        /// ----------------------------------------------------------------------------------------
        static consteval auto max() -> final_t
        {
            return _wrap_final(impl_t::max());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns number of bits this type takes.
        /// ----------------------------------------------------------------------------------------
        static consteval auto bits() -> final_t
        {
            return _wrap_final(impl_t::bits());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the minimum of `this` and `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto min_of(final_t num) const -> final_t
        {
            if (_value > num._value)
                return _wrap_final(num._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the maximum of `this` and `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto max_of(final_t num) const -> final_t
        {
            if (_value < num._value)
                return _wrap_final(num._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// return `this` clamped between `num0` and `num1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto clamp(final_t num0, final_t num1) const -> final_t
        {
            if (_value < num0._value)
                return _wrap_final(num0._value);

            if (_value > num1._value)
                return _wrap_final(num1._value);

            return _clone_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> final_t
            requires(is_signed())
        {
            contract_debug_expects(is_abs_safe());

            return _wrap_final(impl_t::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_checked() const -> final_t
            requires(is_signed())
        {
            contract_expects(is_abs_safe());

            return _wrap_final(impl_t::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_unchecked() const -> final_t
            requires(is_signed())
        {
            return _wrap_final(impl_t::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`abs()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() const -> final_t
            requires(is_signed())
        {
            return abs();
        }

        /// ------------------------>----------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing abs operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_abs_safe() const -> bool
            requires(is_signed())
        {
            return impl_t::is_abs_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg() const -> final_t
            requires(is_signed())
        {
            contract_debug_expects(is_neg_safe());

            return _wrap_final(impl_t::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_checked() const -> final_t
            requires(is_signed())
        {
            contract_expects(is_neg_safe());

            return _wrap_final(impl_t::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_unchecked() const -> final_t
            requires(is_signed())
        {
            return _wrap_final(impl_t::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`neg()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() const -> final_t
            requires(is_signed())
        {
            return neg();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing neg operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg_safe() const -> bool
            requires(is_signed())
        {
            return impl_t::is_neg_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `1` if `this >= 0` else `-1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sign() const -> final_t
            requires(is_signed())
        {
            return _wrap_final(impl_t::sign(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is positive.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_pos() const -> bool
            requires(is_signed())
        {
            return _value >= 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is negative.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg() const -> bool
            requires(is_signed())
        {
            return not is_pos();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq(final_t num) const -> bool
        {
            return _value == num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is less than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_lt(final_t num) const -> bool
        {
            return _value < num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to or less than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_le(final_t num) const -> bool
        {
            return _value <= num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is greater than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_gt(final_t num) const -> bool
        {
            return _value > num._value;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is equal to or greater than `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_ge(final_t num) const -> bool
        {
            return _value >= num._value;
        }

    protected:
        constexpr auto _this_final() const -> const final_t&
        {
            return static_cast<const final_t&>(*this);
        }

        constexpr auto _this_final() -> final_t&
        {
            return static_cast<final_t&>(*this);
        }

        constexpr auto _clone_final() const -> final_t
        {
            return _wrap_final(_value);
        }

        static constexpr auto _wrap_final(unwrapped_t val) -> final_t
        {
            return final_t(val);
        }

    public:
        unwrapped_t _value;
    };
};
