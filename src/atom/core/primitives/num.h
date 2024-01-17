#pragma once
#include "atom/contracts_decl.h"
#include "atom/core/requirements.h"
#include "byte.h"

// #include <cmath>
// #include <numeric>

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
    template <typename tnum>
    concept _rnum = std::is_integral_v<tnum> or std::is_floating_point_v<tnum>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename tnum>
    concept rnum = requires(tnum num) { requires rderived_from<tnum, _num_id>; };

    template <typename tself_, typename tval_, typename tlimit>
    class _num_impl
    {
    public:
        using self_type = tself_;
        using value_type = tval_;

    public:
        static consteval auto min() -> value_type
        {
            return value_type(std::numeric_limits<tlimit>::min());
        }

        static consteval auto max() -> value_type
        {
            return value_type(std::numeric_limits<tlimit>::max());
        }

        static consteval auto count_digits(value_type val) -> size_t
        {
            size_t count = 0;
            while (val > 0)
            {
                val = val / 10;
                count++;
            }

            return count;
        }

        static consteval auto min_digits_count() -> size_t
        {
            return count_digits(min());
        }

        static consteval auto max_digits_count() -> size_t
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
        using self = num<in_impl_type>;

    public:
        using impl_type = in_impl_type;
        using self_type = typename impl_type::self_type;
        using value_type = typename impl_type::value_type;
        using string_type = typename impl_type::string_type;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto min() -> self_type
        {
            return _make(impl_type::min());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto max() -> self_type
        {
            return _make(impl_type::max());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto bits() -> self_type
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
        template <typename tnum>
        static consteval auto is_conversion_safe() -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<tnum>)
            {
                return tnum::template is_assignment_safe<self_type>();
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<tnum>)
                    return false;

                if constexpr (impl_type::min() < std::numeric_limits<tnum>::min())
                    return false;

                if constexpr (impl_type::max() > std::numeric_limits<tnum>::max())
                    return false;

                return true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        static consteval auto is_assignment_safe() -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if constexpr (rnum<tnum>)
            {
                if constexpr (is_signed() != tnum::is_signed())
                    return false;

                if constexpr (tnum::impl_type::min() < impl_type::min())
                    return false;

                if constexpr (tnum::impl_type::max() > impl_type::max())
                    return false;
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<tnum>)
                    return false;

                if constexpr (std::numeric_limits<tnum>::min() < impl_type::min())
                    return false;

                if constexpr (std::numeric_limits<tnum>::max() > impl_type::max())
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
        template <typename tnum>
        constexpr num(tnum num)
            requires(rnum<tnum>) and (is_assignment_safe<tnum>())
        {
            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        explicit constexpr num(tnum num)
            requires(rnum<tnum>) and (not is_assignment_safe<tnum>())
        {
            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr num(tnum num)
            requires(_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator=(tnum num) -> self_type&
            requires(rnum<tnum>) and (is_assignment_safe<tnum>())
        {
            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator=(tnum num) -> self_type&
            requires(_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr operator tnum() const
            requires(_rnum<tnum>) and (self::is_conversion_safe<tnum>())
        {
            return to<tnum>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        explicit constexpr operator tnum() const
            requires(_rnum<tnum>) and (not self::is_conversion_safe<tnum>())
        {
            return to<tnum>();
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
        template <typename tnum>
        constexpr auto assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_assignment(num));

            _val = _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// assigns num.
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
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
        template <typename tnum>
        constexpr auto add(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_add(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator+(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto add_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_add_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::expects(not check_overflow_on_add(num));

            _val += _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator+=(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_add(num));

            return add_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> self_type&
        {
            return add_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() -> self_type
        {
            return _make(+_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto sub(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_sub(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// call [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator-(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto sub_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_sub_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::expects(not check_overflow_on_sub(num));

            _val -= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator-=(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return sub_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`sub_assign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> self_type&
        {
            return sub_assign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() -> self_type
        {
            return _make(-_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto mul(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_mul(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator*(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto mul_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_mul_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::expects(not check_overflow_on_mul(num));

            _val *= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`mul_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator*=(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return mul_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto div(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_div(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _clone().div_assign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator/(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto div_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_div_assign(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::expects(is_div_safe(num));

            _val /= _unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// calls [`div_assign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto operator/=(tnum num) -> self_type&
            requires(rnum<tnum>) or (_rnum<tnum>)
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
        template <typename tnum>
        constexpr auto eq(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _val == _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto ne(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _val != _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto lt(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _val < _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto le(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _val <= _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto gt(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            return _val > _unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto ge(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
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
        constexpr auto count_digits() const -> self_type
        {
            return _make(impl_type::count_digits(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> self_type
        {
            return _make(impl_type::abs(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow() const -> self_type
        {
            return _make(impl_type::pow(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sqrt() const -> self_type
        {
            return _make(impl_type::sqrt(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto root() const -> self_type
        {
            return _make(impl_type::root(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log() const -> self_type
        {
            return _make(impl_type::log(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log10() const -> self_type
        {
            return _make(impl_type::log10(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto min(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if (_val > _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto max(tnum num) const -> self_type
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if (_val < _unwrap(num))
                return _make(_unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum0, typename tnum1>
        constexpr auto clamp(tnum0 num0, tnum1 num1) const -> self_type
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
        constexpr auto clone() const -> self_type
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
        template <typename tnum>
        constexpr auto to() const -> tnum
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::debug_expects(not check_overflow_on_conversion<tnum>());

            return tnum(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto checked_to() const -> tnum
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            contracts::expects(not check_overflow_on_conversion<tnum>());

            return tnum(_val);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// checks
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        static constexpr auto check_overflow_on_assignment(tnum num) -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if constexpr (rnum<tnum>)
            {
                if constexpr (is_signed() != tnum::is_signed())
                    return false;

                if constexpr (tnum::impl_type::min() < impl_type::min())
                    if (_unwrap(num) < impl_type::min())
                        return true;

                if constexpr (tnum::impl_type::max() > impl_type::max())
                    if (_unwrap(num) > impl_type::max())
                        return true;
            }
            else
            {
                if constexpr (is_signed() != std::is_signed_v<tnum>)
                    return false;

                if constexpr (std::numeric_limits<tnum>::min() < impl_type::min())
                    if (num < impl_type::min())
                        return true;

                if constexpr (std::numeric_limits<tnum>::max() > impl_type::max())
                    if (num > impl_type::max())
                        return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto check_overflow_on_conversion() const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            // it's better to ask the target type if it can accept our range values.
            if constexpr (rnum<tnum>)
            {
                return tnum::template check_overflow_on_assignment<self_type>(_val);
            }
            else
            {
                if constexpr (impl_type::min() < std::numeric_limits<tnum>::min())
                    if (_val < std::numeric_limits<tnum>::min())
                        return true;

                if constexpr (impl_type::min() > std::numeric_limits<tnum>::max())
                    if (_val > std::numeric_limits<tnum>::max())
                        return true;

                return false;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto check_overflow_on_add(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if ((impl_type::max() - _val) < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto check_overflow_on_sub(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if ((_val - impl_type::min()) < _unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename tnum>
        constexpr auto check_overflow_on_mul(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
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
        template <typename tnum>
        constexpr auto is_div_safe(tnum num) const -> bool
            requires(rnum<tnum>) or (_rnum<tnum>)
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
        template <typename tout>
        constexpr auto to_string_out(tout&& out) const -> tout&
        {
            return impl_type::to_string_out(_val, forward<tout>(out));
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
        constexpr auto _self() const -> const self_type&
        {
            return static_cast<const self_type&>(*this);
        }

        constexpr auto _self() -> self_type&
        {
            return static_cast<self_type&>(*this);
        }

        constexpr auto _clone() const -> self_type
        {
            return _make(_val);
        }

        static constexpr auto _make(value_type val) -> self_type
        {
            return self_type(val);
        }

        template <typename tnum>
        static constexpr auto _unwrap(tnum num)
            requires(rnum<tnum>) or (_rnum<tnum>)
        {
            if constexpr (rnum<tnum>)
                return num._val;
            else
                return num;
        }

    public:
        value_type _val;
    };
};
