#pragma once
#include "Atom/Contracts.decl.h"
#include "Atom/Core/Requirements.h"
#include "Byte.h"

#include <cmath>
#include <numeric>

/// ------------------------------------------------------------------------------------------------
/// # To Do
///
/// - Needs refactoring.
/// ------------------------------------------------------------------------------------------------
namespace Atom
{
    template <typename T>
    using MemPtr = T*;

    using memunit = byte;
    using memptr = MemPtr<memunit>;

    class _NumId
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TNum>
    concept _RNum = std::is_integral_v<TNum> or std::is_floating_point_v<TNum>;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TNum>
    concept RNum = requires(TNum num) { requires RDerivedFrom<TNum, _NumId>; };

    template <typename TSelf_, typename TVal_, typename TLimit>
    class _NumImpl
    {
    public:
        using TSelf = TSelf_;
        using TVal = TVal_;

    public:
        static consteval auto Min() -> TVal
        {
            return TVal(std::numeric_limits<TLimit>::min());
        }

        static consteval auto Max() -> TVal
        {
            return TVal(std::numeric_limits<TLimit>::max());
        }

        static consteval auto CountDigits(TVal val) -> size_t
        {
            size_t count = 0;
            while (val > 0)
            {
                val = val / 10;
                count++;
            }

            return count;
        }

        static consteval auto MinDigitsCount() -> size_t
        {
            return CountDigits(Min());
        }

        static consteval auto MaxDigitsCount() -> size_t
        {
            return CountDigits(Max());
        }

        static constexpr auto Abs(TVal val) -> TVal
        {
            if constexpr (std::is_unsigned_v<TVal>)
                return val;

            return std::abs(val);
        }

        static consteval auto IsSigned() -> bool
        {
            return std::is_signed_v<TVal>;
        }
    };

    template <typename TImpl_>
    class Num: public _NumId
    {
        using Self = Num<TImpl_>;

    public:
        using TImpl = TImpl_;
        using TSelf = typename TImpl::TSelf;
        using TVal = typename TImpl::TVal;
        using TString = typename TImpl::TString;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto Min() -> TSelf
        {
            return _Make(TImpl::Min());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto Max() -> TSelf
        {
            return _Make(TImpl::Max());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto Bits() -> TSelf
        {
            return _Make(sizeof(TVal) * sizeof(byte));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto IsSigned() -> bool
        {
            return TImpl::IsSigned();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        static consteval auto IsConversionSafe() -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            // It's better to ask the target type if it can accept our range values.
            if constexpr (RNum<TNum>)
            {
                return TNum::template IsAssignmentSafe<TSelf>();
            }
            else
            {
                if constexpr (IsSigned() != std::is_signed_v<TNum>)
                    return false;

                if constexpr (TImpl::Min() < std::numeric_limits<TNum>::min())
                    return false;

                if constexpr (TImpl::Max() > std::numeric_limits<TNum>::max())
                    return false;

                return true;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        static consteval auto IsAssignmentSafe() -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if constexpr (RNum<TNum>)
            {
                if constexpr (IsSigned() != TNum::IsSigned())
                    return false;

                if constexpr (TNum::TImpl::Min() < TImpl::Min())
                    return false;

                if constexpr (TNum::TImpl::Max() > TImpl::Max())
                    return false;
            }
            else
            {
                if constexpr (IsSigned() != std::is_signed_v<TNum>)
                    return false;

                if constexpr (std::numeric_limits<TNum>::min() < TImpl::Min())
                    return false;

                if constexpr (std::numeric_limits<TNum>::max() > TImpl::Max())
                    return false;
            }

            return true;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # To Do
        ///
        /// - No default constructor should be provided. Value should be initialized explicitly.
        /// ----------------------------------------------------------------------------------------
        constexpr Num() = default;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr Num(TNum num)
            requires(RNum<TNum>) and (IsAssignmentSafe<TNum>())
        {
            _val = _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        explicit constexpr Num(TNum num)
            requires(RNum<TNum>) and (not IsAssignmentSafe<TNum>())
        {
            _val = _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr Num(TNum num)
            requires(_RNum<TNum>)
        {
            debug_expects(not CheckOverflowOnAssignment(num));

            _val = _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator=(TNum num) -> TSelf&
            requires(RNum<TNum>) and (IsAssignmentSafe<TNum>())
        {
            _val = _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator=(TNum num) -> TSelf&
            requires(_RNum<TNum>)
        {
            debug_expects(not CheckOverflowOnAssignment(num));

            _val = _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr operator TNum() const
            requires(_RNum<TNum>) and (Self::IsConversionSafe<TNum>())
        {
            return to<TNum>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        explicit constexpr operator TNum() const
            requires(_RNum<TNum>) and (not Self::IsConversionSafe<TNum>())
        {
            return to<TNum>();
        }

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Assignment
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// Assigns num.
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto assign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not CheckOverflowOnAssignment(num));

            _val = _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// Assigns num.
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(not CheckOverflowOnAssignment(num));

            _val = _Unwrap(num);
            return _self();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Arithmetic
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// Adds
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto add(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().addAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedAdd(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().addAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator+(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto addAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not checkOverflowOnAdd(num));

            _val += _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedAddAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(not checkOverflowOnAdd(num));

            _val += _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator+=(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not checkOverflowOnAdd(num));

            return addAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int) -> TSelf&
        {
            return addAssign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() -> TSelf
        {
            return _Make(+_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto sub(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().subAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedSub(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().subAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// Call [`sub(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator-(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto subAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not checkOverflowOnSub(num));

            _val -= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedSubAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(not checkOverflowOnSub(num));

            _val -= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`subAssign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator-=(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return subAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`subAssign(1)`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int) -> TSelf&
        {
            return subAssign(1);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() -> TSelf
        {
            return _Make(-_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto mul(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().mulAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedMul(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().mulAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`mul(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator*(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return mul(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto mulAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not checkOverflowOnMul(num));

            _val *= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedMulAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(not checkOverflowOnMul(num));

            _val *= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`mulAssign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator*=(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return mulAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto div(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().divAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedDiv(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _clone().divAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`div(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator/(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto divAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(isDivSafe(num));

            _val /= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedDivAssign(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(isDivSafe(num));

            _val /= _Unwrap(num);
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls [`divAssign(num)`].
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator/=(TNum num) -> TSelf&
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return divAssign(num);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Bit Operations
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        constexpr auto leftShift() {}

        constexpr auto rightShift() {}

        constexpr auto leftRotate() {}

        constexpr auto rightRotate() {}

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Compairision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto eq(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val == _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto ne(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val != _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto lt(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val < _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto le(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val <= _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto gt(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val > _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto ge(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return _val >= _Unwrap(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator==(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return eq(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator!=(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return ne(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator<(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return lt(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator<=(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return le(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator>(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return gt(num);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto operator>=(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            return ge(num);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Utils
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto countDigits() const -> TSelf
        {
            return _Make(TImpl::CountDigits(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> TSelf
        {
            return _Make(TImpl::Abs(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow() const -> TSelf
        {
            return _Make(TImpl::Pow(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto sqrt() const -> TSelf
        {
            return _Make(TImpl::Sqrt(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto root() const -> TSelf
        {
            return _Make(TImpl::Root(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log() const -> TSelf
        {
            return _Make(TImpl::Log(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto log10() const -> TSelf
        {
            return _Make(TImpl::Log10(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto min(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if (_val > _Unwrap(num))
                return _Make(_Unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto max(TNum num) const -> TSelf
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if (_val < _Unwrap(num))
                return _Make(_Unwrap(num));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum0, typename TNum1>
        constexpr auto clamp(TNum0 num0, TNum1 num1) const -> TSelf
            requires(RNum<TNum0>) or (_RNum<TNum0>) and (RNum<TNum0>) or (_RNum<TNum0>)
        {
            debug_expects(num0 <= num1, "Left of range is greater than the right.");

            if (_val < _Unwrap(num0))
                return _Make(_Unwrap(num0));

            if (_val > _Unwrap(num1))
                return _Make(_Unwrap(num1));

            return _clone();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto clone() const -> TSelf
        {
            return _clone();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Conversion
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto to() const -> TNum
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            debug_expects(not checkOverflowOnConversion<TNum>());

            return TNum(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkedTo() const -> TNum
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            expects(not checkOverflowOnConversion<TNum>());

            return TNum(_val);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Checks
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        static constexpr auto CheckOverflowOnAssignment(TNum num) -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if constexpr (RNum<TNum>)
            {
                if constexpr (IsSigned() != TNum::IsSigned())
                    return false;

                if constexpr (TNum::TImpl::Min() < TImpl::Min())
                    if (_Unwrap(num) < TImpl::Min())
                        return true;

                if constexpr (TNum::TImpl::Max() > TImpl::Max())
                    if (_Unwrap(num) > TImpl::Max())
                        return true;
            }
            else
            {
                if constexpr (IsSigned() != std::is_signed_v<TNum>)
                    return false;

                if constexpr (std::numeric_limits<TNum>::min() < TImpl::Min())
                    if (num < TImpl::Min())
                        return true;

                if constexpr (std::numeric_limits<TNum>::max() > TImpl::Max())
                    if (num > TImpl::Max())
                        return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkOverflowOnConversion() const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            // It's better to ask the target type if it can accept our range values.
            if constexpr (RNum<TNum>)
            {
                return TNum::template CheckOverflowOnAssignment<TSelf>(_val);
            }
            else
            {
                if constexpr (TImpl::Min() < std::numeric_limits<TNum>::min())
                    if (_val < std::numeric_limits<TNum>::min())
                        return true;

                if constexpr (TImpl::Min() > std::numeric_limits<TNum>::max())
                    if (_val > std::numeric_limits<TNum>::max())
                        return true;

                return false;
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkOverflowOnAdd(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if ((TImpl::Max() - _val) < _Unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkOverflowOnSub(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if ((_val - TImpl::Min()) < _Unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto checkOverflowOnMul(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            auto limit = TImpl::Max() - _val;
            auto div = limit / _val;
            if (div < _Unwrap(num))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// 
        /// - Complete implementation.
        /// ----------------------------------------------------------------------------------------
        template <typename TNum>
        constexpr auto isDivSafe(TNum num) const -> bool
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if (_Unwrap(num) == -1 and _val == TImpl::Min())
                return false;

            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// String Conversion
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto toString() const -> TString
        {
            return TImpl::ToString(_val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TOut>
        constexpr auto toStringOut(TOut&& out) const -> TOut&
        {
            return TImpl::ToStringOut(_val, forward<TOut>(out));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Misc
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto val() const -> TVal
        {
            return _val;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Implementaions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        constexpr auto _self() const -> const TSelf&
        {
            return static_cast<const TSelf&>(*this);
        }

        constexpr auto _self() -> TSelf&
        {
            return static_cast<TSelf&>(*this);
        }

        constexpr auto _clone() const -> TSelf
        {
            return _Make(_val);
        }

        static constexpr auto _Make(TVal val) -> TSelf
        {
            return TSelf(val);
        }

        template <typename TNum>
        static constexpr auto _Unwrap(TNum num)
            requires(RNum<TNum>) or (_RNum<TNum>)
        {
            if constexpr (RNum<TNum>)
                return num._val;
            else
                return num;
        }

    public:
        TVal _val;
    };
};
