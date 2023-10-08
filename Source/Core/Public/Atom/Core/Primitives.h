/// # To Do: Needs refactoring.

#pragma once
#include <cstdint>
#include <iostream>
#include <numeric>

#include "LangExtensions.h"

namespace Atom
{
    class Str;

    class _NumId {};

    template <typename T, typename TLimits = std::numeric_limits<T>>
    class Num: public _NumId
    {
        using _T = T;
        using Self = Num<T>;

    private:
        template <typename T_>
        friend class Num;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static consteval auto Min() -> Self
        {
            return Self{ TLimits::min() };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static consteval auto Max() -> Self
        {
            return Self{ TLimits::max() };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static consteval auto Bits() -> Self
        {
            return Self{ sizeof(T) * 8 };
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # To Do: No default constructor should be provided. Value should be initialized explicitly.
        /// ----------------------------------------------------------------------------------------
        // constexpr Num(): _val{} {}
        constexpr Num() = default;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        // constexpr Num(const Self& other):
        //     _val{ other._val } { }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        // constexpr auto operator=(const Self& other) -> Self&
        // {
        //     _val = other._val;
        //     return _self();
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr Num(T num):
            _val{ num } {}

        constexpr Num(_NumId, T num):
            _val{ num } {}

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator=(N num) -> Self&
        {
            return assign(num);
        }

    //// -------------------------------------------------------------------------------------------
    //// 
    //// -------------------------------------------------------------------------------------------

    public:
        template <typename N>
        constexpr auto assign(N num) -> Self&
        {
            _val = num;

            return _self();
        }

        template <typename N>
        constexpr auto checkedAssign(N num) -> Self&
        {
            _val = num;

            return _self();
        }

    //// -------------------------------------------------------------------------------------------
    //// Arithmetic Operations
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto add(N num) const -> Self
        {
            return _clone().addAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedAdd(N num) const -> Self
        {
            return _clone().addAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator+(N num) const -> Self
        {
            return add(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto addAssign(N u) -> Self&
        {
            _val += u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedAddAssign(N u) -> Self&
        {
            _val += u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator+=(N u) -> Self&
        {
            return addAssign(u);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator++(int num) -> Self&
        {
            return addAssign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto sub(N num) const -> Self
        {
            return _clone().subAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedSub(N num) const -> Self
        {
            return _clone().subAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator-(N num) const -> Self
        {
            return sub(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto subAssign(N u) -> Self&
        {
            _val -= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedSubAssign(N u) -> Self&
        {
            _val -= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator-=(N u) -> Self&
        {
            return subAssign(u);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator--(int num) -> Self&
        {
            return subAssign(1);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto mul(N num) const -> Self
        {
            return _clone().mulAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedMul(N num) const -> Self
        {
            return _clone().mulAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator*(N num) const -> Self
        {
            return mul(num);
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedAddAssign(N u) -> Self&
        {
            _val += u;
            return _self();
        }

        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto mulAssign(N u) -> Self&
        {
            _val *= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedMulAssign(N u) -> Self&
        {
            _val *= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator*=(N u) -> Self&
        {
            return mulAssign(u);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto div(N num) const -> Self
        {
            return _clone().divAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedDiv(N num) const -> Self
        {
            return _clone().divAssign(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator/(N num) const -> Self
        {
            return div(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto divAssign(N u) -> Self&
        {
            _val /= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto checkedDivAssign(N u) -> Self&
        {
            _val /= u;
            return _self();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator/=(N u) -> Self&
        {
            return divAssign(u);
        }

    //// -------------------------------------------------------------------------------------------
    //// Bit Operations
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr auto leftShift() {}
        constexpr auto rightShift() {}
        constexpr auto leftRotate() {}
        constexpr auto rightRotate() {}

    //// -------------------------------------------------------------------------------------------
    //// Compairision
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto eq(N num) const -> bool
        {
            return _val == num;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto neq(N num) const -> bool
        {
            return _val != num;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto eqZeroApprox() const -> bool
        {
            return _val == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator==(N num) const -> bool
        {
            return eq(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator!=(N num) const -> bool
        {
            return neq(num);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto cmp(N num) const -> int8_t
            requires(std::derived_from<N, _NumId>)
        {
            if (_val < num._val) return -1;
            if (_val > num._val) return 1;

            return 0;
        }

        constexpr auto cmp(Num<int> num) const -> int8_t
        {
            if (_val < num._val) return -1;
            if (_val > num._val) return 1;

            return 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator<(N num) const -> bool
        {
            return cmp(num) == -1;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator<=(N num) const -> bool
        {
            return cmp(num) == -1 || cmp(num) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator>(N num) const -> bool
        {
            return cmp(num) == 1;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto operator>=(N num) const -> bool
        {
            return cmp(num) == 1 || cmp(num) == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Utils
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto clone() const -> Self
        {
            return _self();
        }

    //// -------------------------------------------------------------------------------------------
    //// Conversion
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        static consteval auto IsConversionSafe() -> bool
            requires(std::derived_from<N, _NumId>)
        {
            if (Min()._val < N::Min()._val) return false;
            if (Max()._val > N::Max()._val) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr auto to() const -> N
        {
            return N{ typename N::_T(_val) };
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        constexpr operator N() const
            requires(IsConversionSafe<N>())
        {
            return to<N>();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        // template <typename N>
        // explicit constexpr operator N() const
        //     requires(not IsConversionSafe<N>())
        // {
        //     return to<N>();
        // }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr operator T() const
        {
            return _val;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename N>
        static consteval auto checkOverflowOnConversion() -> bool
        {
            return false;
        }

    //// -------------------------------------------------------------------------------------------
    //// Str Conversion
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto toStr() -> Str;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TOut>
        constexpr auto toStrOut(TOut&& out) -> TOut&
        {
            return out;
        }

    public:
        constexpr auto val() const -> T
        {
            return _val;
        }

    private:
        constexpr auto _self() const -> const Self&
        {
            return *this;
        }

        constexpr auto _self() -> Self&
        {
            return *this;
        }

        constexpr auto _clone() const -> Self
        {
            return Self{ _self() };
        }

    public:
        T _val;
    };
};

namespace Atom
{
    using _i8 = int8_t;
    using _i16 = int16_t;
    using _i32 = int32_t;
    using _i64 = int64_t;

    using _u8 = uint8_t;
    using _u16 = uint16_t;
    using _u32 = uint32_t;
    using _u64 = uint64_t;

    using _ileast8 = int_least8_t;
    using _ileast16 = int_least16_t;
    using _ileast32 = int_least32_t;
    using _ileast64 = int_least64_t;

    using _uleast8 = uint_least8_t;
    using _uleast16 = uint_least16_t;
    using _uleast32 = uint_least32_t;
    using _uleast64 = uint_least64_t;

    using _ifast8 = int_fast8_t;
    using _ifast16 = int_fast16_t;
    using _ifast32 = int_fast32_t;
    using _ifast64 = int_fast64_t;

    using _ufast8 = uint_fast8_t;
    using _ufast16 = uint_fast16_t;
    using _ufast32 = uint_fast32_t;
    using _ufast64 = uint_fast64_t;

    using byte = unsigned char;

    using char8 = char8_t;
    using char16 = char16_t;
    using char32 = char32_t;

    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    using _isize = std::ptrdiff_t;
    using _usize = std::size_t;

    using i8 = Num<_i8>;
    using i16 = Num<_i16>;
    using i32 = Num<_i32>;
    using i64 = Num<_i64>;

    using u8 = Num<_u8>;
    using u16 = Num<_u16>;
    using u32 = Num<_u32>;
    using u64 = Num<_u64>;

    using ileast8 = Num<_ileast8, std::numeric_limits<_i8>>;
    using ileast16 = Num<_ileast16, std::numeric_limits<_i16>>;
    using ileast32 = Num<_ileast32, std::numeric_limits<_i32>>;
    using ileast64 = Num<_ileast64, std::numeric_limits<_i64>>;

    using uleast8 = Num<_uleast8, std::numeric_limits<_u8>>;
    using uleast16 = Num<_uleast16, std::numeric_limits<_u16>>;
    using uleast32 = Num<_uleast32, std::numeric_limits<_u32>>;
    using uleast64 = Num<_uleast64, std::numeric_limits<_u64>>;

    using ifast8 = Num<_ifast8, std::numeric_limits<_i8>>;
    using ifast16 = Num<_ifast16, std::numeric_limits<_i16>>;
    using ifast32 = Num<_ifast32, std::numeric_limits<_i32>>;
    using ifast64 = Num<_ifast64, std::numeric_limits<_i64>>;

    using ufast8 = Num<_ufast8, std::numeric_limits<_u8>>;
    using ufast16 = Num<_ufast16, std::numeric_limits<_u16>>;
    using ufast32 = Num<_ufast32, std::numeric_limits<_u32>>;
    using ufast64 = Num<_ufast64, std::numeric_limits<_u64>>;

    using f32 = Num<_f32>;
    using f64 = Num<_f64>;
    using f128 = Num<_f128>;

    using isize = Num<_isize>;
    using usize = Num<_usize>;

    class Void {};
}

namespace Atom
{
    using NullPtr = std::nullptr_t;
    // static constexpr NullPtr nullptr;

    using NullVal = std::nullptr_t;
    static constexpr NullVal nullval = NullVal();

    using NullPos = std::nullptr_t;
    static constexpr NullPos nullpos = NullPos();

    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;

    using StdStrView = std::string_view;
}
