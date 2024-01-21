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

    template <typename in_final_type, typename in_value_type, typename limit_type>
    class _num_impl
    {
    public:
        using this_final_type = in_final_type;
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
    class num: public _num_id
    {
        using this_type = num<in_impl_type>;

    public:
        using impl_type = in_impl_type;
        using this_final_type = typename impl_type::this_final_type;
        using value_type = typename impl_type::value_type;
        using string_type = typename impl_type::string_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto min() -> this_final_type
        {
            return _make(impl_type::min());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto max() -> this_final_type
        {
            return _make(impl_type::max());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto bits() -> this_final_type
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
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<num_type>)
            {
                return num_type::template is_assignment_safe<this_final_type>();
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
            requires(rnum<num_type>) or (_rnum<num_type>)
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
        constexpr num() = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr num(num_type num)
            requires(rnum<num_type>) and (is_assignment_safe<num_type>())
        {
            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        explicit constexpr num(num_type num)
            requires(rnum<num_type>) and (not is_assignment_safe<num_type>())
        {
            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr num(num_type num)
            requires(_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator=(num_type num) -> this_final_type&
            requires(rnum<num_type>) and (is_assignment_safe<num_type>())
        {
            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator=(num_type num) -> this_final_type&
            requires(_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr operator num_type() const
            requires(_rnum<num_type>) and (this_type::is_conversion_safe<num_type>())
        {
            return to<num_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        explicit constexpr operator num_type() const
            requires(_rnum<num_type>) and (not this_type::is_conversion_safe<num_type>())
        {
            return to<num_type>();
        }

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// assignment
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// assigns num.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// assigns num.
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _self();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// arithmetic
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// adds
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto add(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_add(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator+(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto add_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_add_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator+=(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            return add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> this_final_type&
        {
            return add_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() -> this_final_type
        {
            return _make(+_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto sub(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_sub(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// call [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator-(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto sub_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_sub_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator-=(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> this_final_type&
        {
            return sub_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() -> this_final_type
        {
            return _make(-_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto mul(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_mul(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator*(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto mul_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_mul_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator*=(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto div(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_div(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator/(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto div_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_div_assign(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto operator/=(num_type num) -> this_final_type&
            requires(rnum<num_type>) or (_rnum<num_type>)
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
        template <typename num_type>
        constexpr auto is_eq(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _val == _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_lt(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _val < _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_le(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _val <= _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_gt(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            return _val > _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto is_ge(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
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
        constexpr auto count_digits() const -> this_final_type
        {
            return _make(impl_type::count_digits(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> this_final_type
        {
            return _make(impl_type::abs(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow() const -> this_final_type
        {
            return _make(impl_type::pow(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sqrt() const -> this_final_type
        {
            return _make(impl_type::sqrt(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto root() const -> this_final_type
        {
            return _make(impl_type::root(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log() const -> this_final_type
        {
            return _make(impl_type::log(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log10() const -> this_final_type
        {
            return _make(impl_type::log10(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto min(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if (_val > _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto max(num_type num) const -> this_final_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if (_val < _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum0, typename tnum1>
        constexpr auto clamp(tnum0 num0, tnum1 num1) const -> this_final_type
            requires(rnum<tnum0>) or (_rnum<tnum0>) and (rnum<tnum0>) or (_rnum<tnum0>)
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
        constexpr auto clone() const -> this_final_type
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
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::debug_expects(not check_overflow_on_conversion<num_type>());

            return num_type(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto checked_to() const -> num_type
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            contracts::expects(not check_overflow_on_conversion<num_type>());

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
        static constexpr auto check_overflow_on_assignment(num_type num) -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
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
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<num_type>)
            {
                return num_type::template check_overflow_on_assignment<this_final_type>(_val);
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
        template <typename num_type>
        constexpr auto check_overflow_on_add(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if ((impl_type::max() - _val) < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto check_overflow_on_sub(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if ((_val - impl_type::min()) < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename num_type>
        constexpr auto check_overflow_on_mul(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
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
        template <typename num_type>
        constexpr auto is_div_safe(num_type num) const -> bool
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if (_unwrap(num) == -1 and _val == impl_type::min())
                return false;

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// string conversion
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_string() const -> string_type
        {
            return impl_type::to_string(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename output_type>
        constexpr auto to_string_out(output_type&& out) const -> output_type&
        {
            return impl_type::to_string_out(_val, forward<output_type>(out));
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
        constexpr auto _self() const -> const this_final_type&
        {
            return static_cast<const this_final_type&>(*this);
        }

        constexpr auto _self() -> this_final_type&
        {
            return static_cast<this_final_type&>(*this);
        }

        constexpr auto _clone() const -> this_final_type
        {
            return _make(_val);
        }

        static constexpr auto _make(value_type val) -> this_final_type
        {
            return this_final_type(val);
        }

        template <typename num_type>
        static constexpr auto _unwrap(num_type num)
            requires(rnum<num_type>) or (_rnum<num_type>)
        {
            if constexpr (rnum<num_type>)
                return num._val;
            else
                return num;
        }

    public:
        value_type _val;
    };
};
