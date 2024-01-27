export module atom.core:core.num_wrapper;
import :core.byte;
import :std;
import :contracts_decl;

/// ------------------------------------------------------------------------------------------------
/// # to do
///
/// - needs refactoring.
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class _num_id
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_type>
    concept _rnum = std::is_integral_v<num_type> or std::is_floating_point_v<num_type>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename num_type>
    concept rnum = requires(num_type num) { requires std::derived_from<num_type, _num_id>; };

    template <typename in_this_final_type, typename in_value_type, typename limit_type>
    class _num_impl
    {
    public:
        using final_type = in_this_final_type;
        using value_type = in_value_type;

    public:
        static consteval auto min() -> value_type
        {
            return value_type(std::numeric_limits<limit_type>::min());
        }

        static consteval auto max() -> value_type
        {
            return value_type(std::numeric_limits<limit_type>::max());
        }

        static consteval auto count_digits(value_type val) -> std::size_t
        {
            std::size_t count = 0;
            while (val > 0)
            {
                val = val / 10;
                count++;
            }

            return count;
        }

        static consteval auto min_digits_count() -> std::size_t
        {
            return count_digits(min());
        }

        static consteval auto max_digits_count() -> std::size_t
        {
            return count_digits(max());
        }

        static constexpr auto abs(value_type val) -> value_type
        {
            if constexpr (std::is_unsigned_v<value_type>)
                return val;

            return std::abs(val);
        }

        static consteval auto is_signed() -> bool
        {
            return std::is_signed_v<value_type>;
        }
    };

    template <typename in_impl_type>
    class num_wrapper: public _num_id
    {
        using this_type = num_wrapper<in_impl_type>;

    public:
        using impl_type = in_impl_type;
        using final_type = typename impl_type::final_type;
        using value_type = typename impl_type::value_type;
        using unwrapped_type = typename impl_type::value_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto min() -> final_type
        {
            return _make(impl_type::min());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto max() -> final_type
        {
            return _make(impl_type::max());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto bits() -> final_type
        {
            return _make(sizeof(value_type) * sizeof(byte));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_signed() -> bool
        {
            return impl_type::is_signed();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static consteval auto is_conversion_safe() -> bool
            requires rnum<num_type> or _rnum<num_type>
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<num_type>)
            {
                return num_type::template is_assignment_safe<final_type>();
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<num_type>)
                    return false;

                if constexpr (impl_type::min() < std::numeric_limits<num_type>::min())
                    return false;

                if constexpr (impl_type::max() > std::numeric_limits<num_type>::max())
                    return false;

                return true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static consteval auto is_assignment_safe() -> bool
            requires rnum<num_type> or _rnum<num_type>
        {
            if constexpr (rnum<num_type>)
            {
                if constexpr (is_signed() != num_type::is_signed())
                    return false;

                if constexpr (num_type::impl_type::min() < impl_type::min())
                    return false;

                if constexpr (num_type::impl_type::max() > impl_type::max())
                    return false;
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<num_type>)
                    return false;

                if constexpr (std::numeric_limits<num_type>::min() < impl_type::min())
                    return false;

                if constexpr (std::numeric_limits<num_type>::max() > impl_type::max())
                    return false;
            }

            return true;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - no default constructor should be provided. value should be initialized explicitly.
        /// ----------------------------------------------------------------------------------------
        constexpr num_wrapper() = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // template <typename num_type>
        // constexpr num_wrapper(num_type num)
        //     requires rnum<num_type> and (is_assignment_safe<num_type>())
        // {
        //     _val = _unwrap(num);
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        explicit constexpr num_wrapper(num_type num)
            requires rnum<num_type>
        //  and (not is_assignment_safe<num_type>())
        {
            // _val = _unwrap(num);
            _val = num._val;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr num_wrapper(num_type num)
            requires _rnum<num_type>
        {
            // contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = num;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // template <typename num_type>
        // constexpr auto operator=(num_type num) -> final_type&
        //     requires rnum<num_type> and (is_assignment_safe<num_type>())
        // {
        //     _val = _unwrap(num);
        //     return _this_final();
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator=(num_type num) -> final_type&
            requires _rnum<num_type>
        {
            // contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = num;
            return _this_final();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// assigns num.
        /// ----------------------------------------------------------------------------------------
        constexpr auto set(final_type num) -> final_type&
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// assigns num.
        /// ----------------------------------------------------------------------------------------
        constexpr auto set_checked(final_type num) -> final_type&
        {
            contracts::expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// adds
        /// ----------------------------------------------------------------------------------------
        constexpr auto add(final_type num) const -> final_type
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_checked(final_type num) const -> final_type
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_unchecked(final_type num) const -> final_type
        {
            return _clone().add_assign_unchecked(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+(final_type num) const -> final_type
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign(final_type num) -> final_type&
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_checked(final_type num) -> final_type&
        {
            contracts::expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto add_assign_unchecked(final_type num) -> final_type&
        {
            _val += _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+=(final_type num) -> final_type&
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            return add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> final_type&
        {
            return add_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() -> final_type
        {
            return _make(+_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub(final_type num) const -> final_type
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_checked(final_type num) const -> final_type
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_unchecked(final_type num) const -> final_type
        {
            return _clone().sub_assign_unchecked(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// call [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-(final_type num) const -> final_type
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign(final_type num) -> final_type&
        {
            contracts::debug_expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_checked(final_type num) -> final_type&
        {
            contracts::expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sub_assign_unchecked(final_type num) -> final_type&
        {
            _val -= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-=(final_type num) -> final_type&
        {
            return sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> final_type&
        {
            return sub_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() -> final_type
        {
            return _make(-_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul(final_type num) const -> final_type
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_checked(final_type num) const -> final_type
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_unchecked(final_type num) const -> final_type
        {
            return _clone().mul_assign_unchecked(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*(final_type num) const -> final_type
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign(final_type num) -> final_type&
        {
            contracts::debug_expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_unchecked(final_type num) -> final_type&
        {
            _val *= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mul_assign_checked(final_type num) -> final_type&
        {
            contracts::expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*=(final_type num) -> final_type&
        {
            return mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div(final_type num) const -> final_type
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_checked(final_type num) const -> final_type
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_unchecked(final_type num) const -> final_type
        {
            return _clone().div_assign_unchecked(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/(final_type num) const -> final_type
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign(final_type num) -> final_type&
        {
            contracts::debug_expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_checked(final_type num) -> final_type&
        {
            contracts::expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto div_assign_unchecked(final_type num) -> final_type&
        {
            _val /= _unwrap(num);
            return _this_final();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator/=(final_type num) -> final_type&
        {
            return div_assign(num);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// bit operations
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        constexpr auto left_shift() {}

        constexpr auto right_shift() {}

        constexpr auto left_rotate() {}

        constexpr auto right_rotate() {}

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// compairision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_eq(final_type num) const -> bool
        {
            return _val == _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_lt(final_type num) const -> bool
        {
            return _val < _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_le(final_type num) const -> bool
        {
            return _val <= _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_gt(final_type num) const -> bool
        {
            return _val > _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_ge(final_type num) const -> bool
        {
            return _val >= _unwrap(num);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// utils
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_digits() const -> final_type
        {
            return _make(impl_type::count_digits(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> final_type
        {
            return _make(impl_type::abs(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow() const -> final_type
        {
            return _make(impl_type::pow(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sqrt() const -> final_type
        {
            return _make(impl_type::sqrt(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto root() const -> final_type
        {
            return _make(impl_type::root(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log() const -> final_type
        {
            return _make(impl_type::log(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log10() const -> final_type
        {
            return _make(impl_type::log10(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto min(final_type num) const -> final_type
        {
            if (_val > _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto max(final_type num) const -> final_type
        {
            if (_val < _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum0, typename tnum1>
        constexpr auto clamp(tnum0 num0, tnum1 num1) const -> final_type
            requires rnum<tnum0> or _rnum<tnum0> and rnum<tnum0> or _rnum<tnum0>
        {
            contracts::debug_expects(num0 <= num1, "left of range is greater than the right.");

            if (_val < _unwrap(num0))
                return _make(_unwrap(num0));

            if (_val > _unwrap(num1))
                return _make(_unwrap(num1));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto clone() const -> final_type
        {
            return _clone();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// conversion
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to() const -> num_type
        {
            contracts::debug_expects(not check_overflow_on_conversion<num_type>());

            return num_type(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_checked() const -> num_type
        {
            contracts::expects(not check_overflow_on_conversion<num_type>());

            return num_type(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto to_unchecked() const -> num_type
        {
            return num_type(_val);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// checks
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        static constexpr auto check_overflow_on_assignment(final_type num) -> bool
        {
            if constexpr (rnum<num_type>)
            {
                if constexpr (is_signed() != num_type::is_signed())
                    return false;

                if constexpr (num_type::impl_type::min() < impl_type::min())
                    if (_unwrap(num) < impl_type::min())
                        return true;

                if constexpr (num_type::impl_type::max() > impl_type::max())
                    if (_unwrap(num) > impl_type::max())
                        return true;
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<num_type>)
                    return false;

                if constexpr (std::numeric_limits<num_type>::min() < impl_type::min())
                    if (num < impl_type::min())
                        return true;

                if constexpr (std::numeric_limits<num_type>::max() > impl_type::max())
                    if (num > impl_type::max())
                        return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto check_overflow_on_conversion() const -> bool
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<num_type>)
            {
                return num_type::template check_overflow_on_assignment<final_type>(_val);
            }
            else
            {
                if constexpr (impl_type::min() < std::numeric_limits<num_type>::min())
                    if (_val < std::numeric_limits<num_type>::min())
                        return true;

                if constexpr (impl_type::min() > std::numeric_limits<num_type>::max())
                    if (_val > std::numeric_limits<num_type>::max())
                        return true;

                return false;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_overflow_on_add(final_type num) const -> bool
        {
            if ((impl_type::max() - _val) < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_overflow_on_sub(final_type num) const -> bool
        {
            // todo: fix this implementation.
            //
            // if ((_val - impl_type::min()) < _unwrap(num))
            //     return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_overflow_on_mul(final_type num) const -> bool
        {
            auto limit = impl_type::max() - _val;
            auto div = limit / _val;
            if (div < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - complete implementation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_div_safe(final_type num) const -> bool
        {
            if (_unwrap(num) == -1 and _val == impl_type::min())
                return false;

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// misc
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto unwrap() const -> value_type
        {
            return _val;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// implementaions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        constexpr auto _this_final() const -> const final_type&
        {
            return static_cast<const final_type&>(*this);
        }

        constexpr auto _this_final() -> final_type&
        {
            return static_cast<final_type&>(*this);
        }

        constexpr auto _clone() const -> final_type
        {
            return _make(_val);
        }

        static constexpr auto _make(value_type val) -> final_type
        {
            return final_type(val);
        }

        static constexpr auto _unwrap(final_type num)
        {
            // if constexpr (rnum<num_type>)
            return num._val;
            // else
            //     return num;
        }

    public:
        value_type _val;
    };
};
