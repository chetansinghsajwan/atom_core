#pragma once
#include "Num.h"

#include <cstdint>

namespace Atom
{
    template <size_t>
    class IntString;

    template <typename TSelf, typename TVal, typename TLimit = TVal>
    class _IntImpl: public _NumImpl<TSelf, TVal, TLimit>
    {
        using Base = _NumImpl<TSelf, TVal, TLimit>;
        using Self = _IntImpl<TSelf, TVal, TLimit>;

    public:
        using TString = IntString<Base::MaxDigitsCount() + 1>;

    public:
        static constexpr auto ToString(TVal val) -> TString;

        template <typename TOut>
        static constexpr auto ToStringOut(TVal val, TOut&& out) -> TOut
        {
            TString str = ToString(val);
            out += str;
            return out;
        }

    private:
        static constexpr auto _ToString(TVal val, char* str) -> char*
        {
            bool neg = false;
            if constexpr (std::is_signed_v<TVal>)
            {
                if (val < 0)
                {
                    val = val * -1;
                    neg = true;
                }
            }

            while (val > 0)
            {
                str--;
                *str = '0' + (val % 10);
                val = val / 10;
            }

            if (neg)
            {
                str--;
                *str = '-';
            }

            return str;
        }
    };

    class _IntId
    {};

    template <typename TImpl>
    class Int: public Num<TImpl>, public _IntId
    {
        using Base = Num<TImpl>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    template <typename TInt>
    concept RInt = requires
    {
        requires RDerivedFrom<TInt, _IntId>;
    };

    template <typename TInt, typename T>
    constexpr auto operator+(TInt num, T* ptr) -> T*
        requires(RInt<TInt>)
    {
        return ptr + num._val;
    }

    template <typename TInt, typename T>
    constexpr auto operator-(TInt num, T* ptr) -> T*
        requires(RInt<TInt>)
    {
        return ptr - num._val;
    }

    template <typename TInt, typename T>
    constexpr auto operator+(T* ptr, TInt num) -> T*
        requires(RInt<TInt>)
    {
        return ptr + num._val;
    }

    template <typename TInt, typename T>
    constexpr auto operator-(T* ptr, TInt num) -> T*
        requires(RInt<TInt>)
    {
        return ptr - num._val;
    }

    using _i8 = std::int8_t;
    using _i16 = std::int16_t;
    using _i32 = std::int32_t;
    using _i64 = std::int64_t;

    using _u8 = std::uint8_t;
    using _u16 = std::uint16_t;
    using _u32 = std::uint32_t;
    using _u64 = std::uint64_t;

    using _ileast8 = std::int_least8_t;
    using _ileast16 = std::int_least16_t;
    using _ileast32 = std::int_least32_t;
    using _ileast64 = std::int_least64_t;

    using _uleast8 = std::uint_least8_t;
    using _uleast16 = std::uint_least16_t;
    using _uleast32 = std::uint_least32_t;
    using _uleast64 = std::uint_least64_t;

    using _ifast8 = std::int_fast8_t;
    using _ifast16 = std::int_fast16_t;
    using _ifast32 = std::int_fast32_t;
    using _ifast64 = std::int_fast64_t;

    using _ufast8 = std::uint_fast8_t;
    using _ufast16 = std::uint_fast16_t;
    using _ufast32 = std::uint_fast32_t;
    using _ufast64 = std::uint_fast64_t;

    using _imax = std::intmax_t;
    using _umax = std::uintmax_t;

    using _isize = std::ptrdiff_t;
    using _usize = std::size_t;

    class i8: public Int<_IntImpl<i8, _i8>>
    {
        using Base = Int<_IntImpl<i8, _i8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class i16: public Int<_IntImpl<i16, _i16>>
    {
        using Base = Int<_IntImpl<i16, _i16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class i32: public Int<_IntImpl<i32, _i32>>
    {
        using Base = Int<_IntImpl<i32, _i32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class i64: public Int<_IntImpl<i64, _i64>>
    {
        using Base = Int<_IntImpl<i64, _i64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class u8: public Int<_IntImpl<u8, _u8>>
    {
        using Base = Int<_IntImpl<u8, _u8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class u16: public Int<_IntImpl<u16, _u16>>
    {
        using Base = Int<_IntImpl<u16, _u16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class u32: public Int<_IntImpl<u32, _u32>>
    {
        using Base = Int<_IntImpl<u32, _u32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class u64: public Int<_IntImpl<u64, _u64>>
    {
        using Base = Int<_IntImpl<u64, _u64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ileast8: public Int<_IntImpl<ileast8, _ileast8, _i8>>
    {
        using Base = Int<_IntImpl<ileast8, _ileast8, _i8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ileast16: public Int<_IntImpl<ileast16, _ileast16, _i16>>
    {
        using Base = Int<_IntImpl<ileast16, _ileast16, _i16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ileast32: public Int<_IntImpl<ileast32, _ileast32, _i32>>
    {
        using Base = Int<_IntImpl<ileast32, _ileast32, _i32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ileast64: public Int<_IntImpl<ileast64, _ileast64, _i64>>
    {
        using Base = Int<_IntImpl<ileast64, _ileast64, _i64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class uleast8: public Int<_IntImpl<uleast8, _uleast8, _u8>>
    {
        using Base = Int<_IntImpl<uleast8, _uleast8, _u8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class uleast16: public Int<_IntImpl<uleast16, _uleast16, _u16>>
    {
        using Base = Int<_IntImpl<uleast16, _uleast16, _u16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class uleast32: public Int<_IntImpl<uleast32, _uleast32, _u32>>
    {
        using Base = Int<_IntImpl<uleast32, _uleast32, _u32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class uleast64: public Int<_IntImpl<uleast64, _uleast64, _u64>>
    {
        using Base = Int<_IntImpl<uleast64, _uleast64, _u64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ifast8: public Int<_IntImpl<ifast8, _ifast8, _i8>>
    {
        using Base = Int<_IntImpl<ifast8, _ifast8, _i8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ifast16: public Int<_IntImpl<ifast16, _ifast16, _i16>>
    {
        using Base = Int<_IntImpl<ifast16, _ifast16, _i16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ifast32: public Int<_IntImpl<ifast32, _ifast32, _i32>>
    {
        using Base = Int<_IntImpl<ifast32, _ifast32, _i32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ifast64: public Int<_IntImpl<ifast64, _ifast64, _i64>>
    {
        using Base = Int<_IntImpl<ifast64, _ifast64, _i64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ufast8: public Int<_IntImpl<ufast8, _ufast8, _u8>>
    {
        using Base = Int<_IntImpl<ufast8, _ufast8, _u8>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ufast16: public Int<_IntImpl<ufast16, _ufast16, _u16>>
    {
        using Base = Int<_IntImpl<ufast16, _ufast16, _u16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ufast32: public Int<_IntImpl<ufast32, _ufast32, _u32>>
    {
        using Base = Int<_IntImpl<ufast32, _ufast32, _u32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class ufast64: public Int<_IntImpl<ufast64, _ufast64, _u64>>
    {
        using Base = Int<_IntImpl<ufast64, _ufast64, _u64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class imax: public Int<_IntImpl<imax, _imax>>
    {
        using Base = Int<_IntImpl<imax, _imax>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class umax: public Int<_IntImpl<umax, _umax>>
    {
        using Base = Int<_IntImpl<umax, _umax>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class isize: public Int<_IntImpl<isize, _isize>>
    {
        using Base = Int<_IntImpl<isize, _isize>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class usize: public Int<_IntImpl<usize, _usize>>
    {
        using Base = Int<_IntImpl<usize, _usize>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
};
