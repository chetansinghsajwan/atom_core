#pragma once
#include "num.h"

// #include <cstdint>

/// ------------------------------------------------------------------------------------------------
/// # to do
///
/// - refactor this.
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class _int_id
    {};

    template <typename tint>
    concept _rint = std::is_integral_v<tint>;

    template <typename tint>
    concept rint = std::derived_from<tint, _int_id>;

    template <typename tint>
    constexpr auto _unwrap_int(tint n)
        requires rint<tint> or _rint<tint>
    {
        if constexpr (rint<tint>)
        {
            return n.unwrap();
        }
        else
        {
            return n;
        }
    }

    template <size_t>
    class int_string;

    template <typename self_type, typename value_type, typename tlimit = value_type>
    class _int_impl: public _num_impl<self_type, value_type, tlimit>
    {
        using base_type = _num_impl<self_type, value_type, tlimit>;
        using self = _int_impl<self_type, value_type, tlimit>;

    public:
        using string_type = int_string<base_type::max_digits_count() + 1>;

    public:
        static constexpr auto to_string(value_type val) -> string_type;

        template <typename tout>
        static constexpr auto to_string_out(value_type val, tout&& out) -> tout
        {
            string_type str = to_string(val);
            out += str;
            return out;
        }

    private:
        static constexpr auto _to_string(value_type val, uchar* str) -> uchar*
        {
            bool neg = false;
            if constexpr (std::is_signed_v<value_type>)
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

    template <typename impl_type>
    class _int
        : public num<impl_type>
        , public _int_id
    {
        using base_type = num<impl_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

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

    class i8: public _int<_int_impl<i8, _i8>>
    {
        using base_type = _int<_int_impl<i8, _i8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class i16: public _int<_int_impl<i16, _i16>>
    {
        using base_type = _int<_int_impl<i16, _i16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class i32: public _int<_int_impl<i32, _i32>>
    {
        using base_type = _int<_int_impl<i32, _i32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class i64: public _int<_int_impl<i64, _i64>>
    {
        using base_type = _int<_int_impl<i64, _i64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class u8: public _int<_int_impl<u8, _u8>>
    {
        using base_type = _int<_int_impl<u8, _u8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class u16: public _int<_int_impl<u16, _u16>>
    {
        using base_type = _int<_int_impl<u16, _u16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class u32: public _int<_int_impl<u32, _u32>>
    {
        using base_type = _int<_int_impl<u32, _u32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class u64: public _int<_int_impl<u64, _u64>>
    {
        using base_type = _int<_int_impl<u64, _u64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ileast8: public _int<_int_impl<ileast8, _ileast8, _i8>>
    {
        using base_type = _int<_int_impl<ileast8, _ileast8, _i8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ileast16: public _int<_int_impl<ileast16, _ileast16, _i16>>
    {
        using base_type = _int<_int_impl<ileast16, _ileast16, _i16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ileast32: public _int<_int_impl<ileast32, _ileast32, _i32>>
    {
        using base_type = _int<_int_impl<ileast32, _ileast32, _i32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ileast64: public _int<_int_impl<ileast64, _ileast64, _i64>>
    {
        using base_type = _int<_int_impl<ileast64, _ileast64, _i64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class uleast8: public _int<_int_impl<uleast8, _uleast8, _u8>>
    {
        using base_type = _int<_int_impl<uleast8, _uleast8, _u8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class uleast16: public _int<_int_impl<uleast16, _uleast16, _u16>>
    {
        using base_type = _int<_int_impl<uleast16, _uleast16, _u16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class uleast32: public _int<_int_impl<uleast32, _uleast32, _u32>>
    {
        using base_type = _int<_int_impl<uleast32, _uleast32, _u32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class uleast64: public _int<_int_impl<uleast64, _uleast64, _u64>>
    {
        using base_type = _int<_int_impl<uleast64, _uleast64, _u64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ifast8: public _int<_int_impl<ifast8, _ifast8, _i8>>
    {
        using base_type = _int<_int_impl<ifast8, _ifast8, _i8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ifast16: public _int<_int_impl<ifast16, _ifast16, _i16>>
    {
        using base_type = _int<_int_impl<ifast16, _ifast16, _i16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ifast32: public _int<_int_impl<ifast32, _ifast32, _i32>>
    {
        using base_type = _int<_int_impl<ifast32, _ifast32, _i32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ifast64: public _int<_int_impl<ifast64, _ifast64, _i64>>
    {
        using base_type = _int<_int_impl<ifast64, _ifast64, _i64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ufast8: public _int<_int_impl<ufast8, _ufast8, _u8>>
    {
        using base_type = _int<_int_impl<ufast8, _ufast8, _u8>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ufast16: public _int<_int_impl<ufast16, _ufast16, _u16>>
    {
        using base_type = _int<_int_impl<ufast16, _ufast16, _u16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ufast32: public _int<_int_impl<ufast32, _ufast32, _u32>>
    {
        using base_type = _int<_int_impl<ufast32, _ufast32, _u32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class ufast64: public _int<_int_impl<ufast64, _ufast64, _u64>>
    {
        using base_type = _int<_int_impl<ufast64, _ufast64, _u64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class imax: public _int<_int_impl<imax, _imax>>
    {
        using base_type = _int<_int_impl<imax, _imax>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class umax: public _int<_int_impl<umax, _umax>>
    {
        using base_type = _int<_int_impl<umax, _umax>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class isize: public _int<_int_impl<isize, _isize>>
    {
        using base_type = _int<_int_impl<isize, _isize>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class usize: public _int<_int_impl<usize, _usize>>
    {
        using base_type = _int<_int_impl<usize, _usize>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
};
