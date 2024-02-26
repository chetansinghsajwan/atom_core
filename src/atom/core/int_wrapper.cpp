module;
#include "atom/preprocessors.h"

export module atom.core:core.int_wrapper;
import :core.char_wrapper;
import :core.num_wrapper;
import :core.byte;
import :contracts_decl;
import :std;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// common implementation for signed and unsigned integers.
    /// --------------------------------------------------------------------------------------------
    template <typename in_final_type, typename in_signed_type, typename in_unsigned_type,
        typename in_unwrapped_type, typename limit_type>
    class _int_wrapper_impl
    {
    public:
        using final_type = in_final_type;
        using unwrapped_type = in_unwrapped_type;
        using signed_type = in_signed_type;
        using unsigned_type = in_unsigned_type;

    public:
        static consteval auto min() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<limit_type>::min());
        }

        static consteval auto max() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<limit_type>::max());
        }

        static consteval auto bits() -> unwrapped_type
        {
            return unwrapped_type(sizeof(limit_type) * byte_get_bit_count());
        }

        static consteval auto is_signed() -> bool
        {
            return std::is_signed_v<unwrapped_type>;
        }

        static constexpr auto count_digits(unwrapped_type num) -> unwrapped_type
        {
            if (num == 0)
                return 1;

            if constexpr (is_signed())
                return std::log10(std::abs(num)) + 1;
            else
                return std::log10(num) + 1;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// implementation for unsigned integers.
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename signed_type, typename unwrapped_type,
        typename limit_type>
    class _unsigned_int_wrapper_impl
        : public _int_wrapper_impl<final_type, signed_type, final_type, unwrapped_type, limit_type>
    {
        using base_type =
            _int_wrapper_impl<final_type, signed_type, final_type, unwrapped_type, limit_type>;

    public:
        static constexpr auto is_add_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return rhs <= base_type::max() - lhs;
        }

        static constexpr auto is_sub_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return rhs <= lhs - base_type::min();
        }

        static constexpr auto is_mul_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if (rhs > 0 && lhs > base_type::max() / rhs)
                return false;

            return true;
        }

        static constexpr auto is_div_safe(unwrapped_type num, unwrapped_type den) -> bool
        {
            return true;
        }

        static constexpr auto is_abs_safe(unwrapped_type num) -> bool
        {
            return true;
        }

        static constexpr auto is_neg_safe(unwrapped_type num) -> bool
        {
            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
        {
            return is_conversion_safe_from_unwrapped<typename num_type::unwrapped_type>(
                num.to_unwrapped());
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
        {
            if constexpr (std::is_signed_v<num_type>)
                if (num < 0)
                    return false;

            if constexpr (std::numeric_limits<num_type>::max() > base_type::max())
                if (num > base_type::max())
                    return false;

            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_to_unwrapped(unwrapped_type num) -> bool
        {
            if constexpr (base_type::max() > std::numeric_limits<num_type>::max())
            {
                if (num > std::numeric_limits<num_type>::max())
                    return false;
            }

            return true;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// implementation for signed integers.
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename unsigned_type, typename unwrapped_type,
        typename limit_type>
    class _signed_int_wrapper_impl
        : public _int_wrapper_impl<final_type, final_type, unsigned_type, unwrapped_type,
              limit_type>
    {
        using base_type =
            _int_wrapper_impl<final_type, final_type, unsigned_type, unwrapped_type, limit_type>;

    public:
        static constexpr auto abs(unwrapped_type num) -> unwrapped_type
        {
            return std::abs(num);
        }

        static constexpr auto neg(unwrapped_type num) -> unwrapped_type
        {
            return -num;
        }

        static constexpr auto sign(unwrapped_type num) -> unwrapped_type
        {
            return num < 0 ? -1 : 1;
        }

        static constexpr auto is_add_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if (rhs > 0 && lhs > base_type::max() - rhs)
                return false;

            if (rhs < 0 && lhs < base_type::min() - rhs)
                return false;

            return true;
        }

        static constexpr auto is_sub_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            return is_add_safe(lhs, -rhs);
        }

        static constexpr auto is_mul_safe(unwrapped_type lhs, unwrapped_type rhs) -> bool
        {
            if (rhs > 0 && lhs > base_type::max() / rhs)
                return false;

            if (rhs < 0 && lhs < base_type::min() / rhs)
                return false;

            return true;
        }

        static constexpr auto is_div_safe(unwrapped_type num, unwrapped_type den) -> bool
        {
            if (den == 0)
                return false;

            if (num == base_type::min() && den == -1)
                return false;

            return true;
        }

        static constexpr auto is_abs_safe(unwrapped_type num) -> bool
        {
            return num != base_type::min();
        }

        static constexpr auto is_neg_safe(unwrapped_type num) -> bool
        {
            return num != base_type::min();
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from(num_type num) -> bool
        {
            return is_conversion_safe_from_unwrapped<typename num_type::unwrapped_type>(
                num.to_unwrapped());
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_from_unwrapped(num_type num) -> bool
        {
            if constexpr (std::is_signed_v<num_type>
                          and std::numeric_limits<num_type>::min() < base_type::min())
            {
                if (num < base_type::min())
                {
                    return false;
                }
            }

            if constexpr (std::numeric_limits<num_type>::max() > base_type::max())
            {
                if (num > base_type::max())
                {
                    return false;
                }
            }

            return true;
        }

        template <typename num_type>
        static constexpr auto is_conversion_safe_to_unwrapped(unwrapped_type num) -> bool
        {
            if constexpr (base_type::min() < std::numeric_limits<num_type>::min())
                if (num < std::numeric_limits<num_type>::min())
                    return false;

            if constexpr (base_type::max() > std::numeric_limits<num_type>::max())
                if (num > std::numeric_limits<num_type>::max())
                    return false;

            return true;
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// wrapper for integer types.
    /// --------------------------------------------------------------------------------------------
    template <typename impl_type>
    class int_wrapper: public num_wrapper<impl_type>
    {
        using base_type = num_wrapper<impl_type>;
        using final_type = typename impl_type::final_type;
        using signed_type = typename impl_type::signed_type;
        using unsigned_type = typename impl_type::unsigned_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this is a signed integer type.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_signed() -> bool
        {
            return impl_type::is_signed();
        }

        /// ----------------------------------------------------------------------------------------
        /// counts number of digits needed to represent `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_digits() const -> final_type
        {
            return _wrap_final(impl_type::count_digits(_value));
        }

    public:
        using base_type::_value;
        using base_type::_wrap_final;
    };

    /// --------------------------------------------------------------------------------------------
    /// wrapper for unsigned integer types.
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename signed_type, typename unwrapped_type,
        typename limit_type = unwrapped_type>
    class unsigned_int_wrapper
        : public int_wrapper<
              _unsigned_int_wrapper_impl<final_type, signed_type, unwrapped_type, limit_type>>
    {
        using base_type = int_wrapper<
            _unsigned_int_wrapper_impl<final_type, signed_type, unwrapped_type, limit_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    /// wrapper for signed integer types.
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename unsigned_type, typename unwrapped_type,
        typename limit_type = unwrapped_type>
    class signed_int_wrapper
        : public int_wrapper<
              _signed_int_wrapper_impl<final_type, unsigned_type, unwrapped_type, limit_type>>
    {
        using base_type = int_wrapper<
            _signed_int_wrapper_impl<final_type, unsigned_type, unwrapped_type, limit_type>>;

    protected:
        using impl_type = typename base_type::impl_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is positive.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_pos() const -> bool
        {
            return _value >= 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if `this` is negative.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg() const -> bool
        {
            return not is_pos();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs() const -> final_type
        {
            contracts::debug_expects(is_abs_safe());

            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_checked() const -> final_type
        {
            contracts::expects(is_abs_safe());

            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns absolute value of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto abs_unchecked() const -> final_type
        {
            return _wrap_final(impl_type::abs(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`abs()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator+() const -> final_type
        {
            return abs();
        }

        using base_type::operator+;

        /// ------------------------>----------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing abs operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_abs_safe() const -> bool
        {
            return impl_type::is_abs_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg() const -> final_type
        {
            contracts::debug_expects(is_neg_safe());

            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_checked() const -> final_type
        {
            contracts::expects(is_neg_safe());

            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns result after reversing sign of `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto neg_unchecked() const -> final_type
        {
            return _wrap_final(impl_type::neg(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns [`neg()`].
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator-() const -> final_type
        {
            return neg();
        }

        using base_type::operator-;

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if there is no overflow or underflow when performing neg operation.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg_safe() const -> bool
        {
            return impl_type::is_neg_safe(_value);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `1` if `this >= 0` else `-1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sign() const -> final_type
        {
            return _wrap_final(impl_type::sign(_value));
        }

    public:
        using base_type::_value;
        using base_type::_wrap_final;
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    using _i8 = std::int8_t;
    using _i16 = std::int16_t;
    using _i32 = std::int32_t;
    using _i64 = std::int64_t;
    using _ifast8 = std::int_fast8_t;
    using _ifast16 = std::int_fast16_t;
    using _ifast32 = std::int_fast32_t;
    using _ifast64 = std::int_fast64_t;
    using _imax = std::intmax_t;
    using _isize = std::ptrdiff_t;

    using _u8 = std::uint8_t;
    using _u16 = std::uint16_t;
    using _u32 = std::uint32_t;
    using _u64 = std::uint64_t;
    using _ufast8 = std::uint_fast8_t;
    using _ufast16 = std::uint_fast16_t;
    using _ufast32 = std::uint_fast32_t;
    using _ufast64 = std::uint_fast64_t;
    using _umax = std::uintmax_t;
    using _usize = std::size_t;

    class i8;
    class i16;
    class i32;
    class i64;
    class ifast8;
    class ifast16;
    class ifast32;
    class ifast64;
    class imax;
    class isize;

    class u8;
    class u16;
    class u32;
    class u64;
    class ufast8;
    class ufast16;
    class ufast32;
    class ufast64;
    class umax;
    class usize;

    ATOM_ALIAS(i8, signed_int_wrapper<i8, u8, _i8>);
    ATOM_ALIAS(i16, signed_int_wrapper<i16, u16, _i16>);
    ATOM_ALIAS(i32, signed_int_wrapper<i32, u32, _i32>);
    ATOM_ALIAS(i64, signed_int_wrapper<i64, u64, _i64>);
    ATOM_ALIAS(ifast8, signed_int_wrapper<ifast8, ufast8, _ifast8, _i8>);
    ATOM_ALIAS(ifast16, signed_int_wrapper<ifast16, ufast16, _ifast16, _i16>);
    ATOM_ALIAS(ifast32, signed_int_wrapper<ifast32, ufast32, _ifast32, _i32>);
    ATOM_ALIAS(ifast64, signed_int_wrapper<ifast64, ufast64, _ifast64, _i64>);
    ATOM_ALIAS(imax, signed_int_wrapper<imax, umax, _imax>);
    ATOM_ALIAS(isize, signed_int_wrapper<isize, usize, _isize>);

    ATOM_ALIAS(u8, unsigned_int_wrapper<u8, i8, _u8>);
    ATOM_ALIAS(u16, unsigned_int_wrapper<u16, i16, _u16>);
    ATOM_ALIAS(u32, unsigned_int_wrapper<u32, i32, _u32>);
    ATOM_ALIAS(u64, unsigned_int_wrapper<u64, i64, _u64>);
    ATOM_ALIAS(ufast8, unsigned_int_wrapper<ufast8, ifast8, _ufast8, _u8>);
    ATOM_ALIAS(ufast16, unsigned_int_wrapper<ufast16, ifast16, _ufast16, _u16>);
    ATOM_ALIAS(ufast32, unsigned_int_wrapper<ufast32, ifast32, _ufast32, _u32>);
    ATOM_ALIAS(ufast64, unsigned_int_wrapper<ufast64, ifast64, _ufast64, _u64>);
    ATOM_ALIAS(umax, unsigned_int_wrapper<umax, imax, _umax>);
    ATOM_ALIAS(usize, unsigned_int_wrapper<usize, isize, _usize>);
}
