module;
#include "atom/preprocessors.h"

export module atom.core:core.int_wrapper;
import :core.char_wrapper;
import :core.num_wrapper;
import :contracts_decl;
import :std;

/// ------------------------------------------------------------------------------------------------
/// # to do
///
/// - refactor this.
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class _int_id
    {};

    template <typename int_type>
    concept _rint = std::is_integral_v<int_type>;

    template <typename int_type>
    concept rint = std::derived_from<int_type, _int_id>;

    template <typename int_type>
    constexpr auto _unwrap_int(int_type n)
        requires rint<int_type> or _rint<int_type>
    {
        if constexpr (rint<int_type>)
        {
            return n.unwrap();
        }
        else
        {
            return n;
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename in_unsigned_type, typename unwrapped_type,
        typename limit_type = unwrapped_type>
    class int_wrapper
        : public num_wrapper<_num_wrapper_impl<final_type, unwrapped_type, limit_type>>
        , public _int_id
    {
        using base_type = num_wrapper<_num_wrapper_impl<final_type, unwrapped_type, limit_type>>;

    public:
        using impl_type = typename base_type::impl_type;
        using unsigned_type = in_unsigned_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        /// counts number of digits needed to represent `this`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count_digits() const -> final_type
        {
            return _wrap_final(impl_type::count_digits(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` raised to the power of `exp`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow(unsigned_type exp) const -> final_type
        {
            contracts::debug_expects(check_pow());

            return _wrap_final(impl_type::pow(_value, exp.unwrap()));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` raised to the power of `exp`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow_checked(unsigned_type exp) const -> final_type
        {
            contracts::expects(check_pow());

            return _wrap_final(impl_type::pow(_value, exp.unwrap()));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `this` raised to the power of `exp`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto pow_unchecked(unsigned_type exp) const -> final_type
        {
            return _wrap_final(impl_type::pow(_value, exp.unwrap()));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` is performing `pow` operations results in overflow or underflow.
        ///
        /// # todo:
        ///
        /// - implement this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_pow(unsigned_type exp) const -> final_type
        {
            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `num`th root of `this`. If this is an integer, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto root(unsigned_type num) const -> final_type
        {
            return _wrap_final(impl_type::root(_value, num.unwrap()));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns square root of `this`. If this is an integer, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto root2() const -> final_type
        {
            return _wrap_final(impl_type::root2(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns cube root of `this`. If this is an integer, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto root3() const -> final_type
        {
            return _wrap_final(impl_type::root3(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the logarithm of the number with respect to an arbitrary base `num`, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto log(unsigned_type num) const -> final_type
        {
            return _wrap_final(impl_type::log(_value, num.unwrap()));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the base 2 logarithm of the number, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto log2() const -> final_type
        {
            return _wrap_final(impl_type::log2(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the base 2 logarithm of the number, rounded down.
        /// ----------------------------------------------------------------------------------------
        constexpr auto log10() const -> final_type
        {
            return _wrap_final(impl_type::log10(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts bits left by `num` and returns result.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::shift_left(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left_checked(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::shift_left(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_left_unchecked(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::shift_left(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts bits right by `num` and returns result.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_right(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::shift_right(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// shifts bits by `num` and returns result.
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by(final_type num) -> final_type
        {
            return _wrap_final(impl_type::shift_by(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// rotate bits left by `num` and returns result.
        /// ----------------------------------------------------------------------------------------
        constexpr auto roatate_left(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::roatate_left(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// rotate bits right by `num` and returns result.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_right(unsigned_type num) -> final_type
        {
            return _wrap_final(impl_type::rotate_right(_value));
        }

        /// ----------------------------------------------------------------------------------------
        /// rotate bits by `num`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_by(final_type num) -> final_type
        {
            return _wrap_final(impl_type::rotate_by(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // constexpr auto to_float() -> float_type {}

    public:
        using base_type::_value;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename in_unsigned_type, typename unwrapped_type,
        typename limit_type = unwrapped_type>
    class signed_int_wrapper
        : public int_wrapper<final_type, in_unsigned_type, unwrapped_type, limit_type>
    {
        using base_type = int_wrapper<final_type, in_unsigned_type, unwrapped_type, limit_type>;

    protected:
        using impl_type = typename base_type::impl_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
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

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_abs_safe() const -> bool
        {
            return true;
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
        /// returns result after reversing sign of `this`. alway sperforms checks.
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
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_neg_safe() const -> bool
        {
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `1` if `this >= 0` else `-1`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto sign() const -> final_type
        {
            return _wrap_final(impl_type::sign(_value));
        }

    public:
        using base_type::_wrap_final;
        using base_type::_value;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename final_type, typename in_signed_type, typename unwrapped_type,
        typename limit_type = unwrapped_type>
    class unsigned_int_wrapper
        : public int_wrapper<final_type, final_type, unwrapped_type, limit_type>
    {
        using base_type = int_wrapper<final_type, final_type, unwrapped_type, limit_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}

export namespace atom
{
    using _i8 = std::int8_t;
    using _i16 = std::int16_t;
    using _i32 = std::int32_t;
    using _i64 = std::int64_t;
    using _ileast8 = std::int_least8_t;
    using _ileast16 = std::int_least16_t;
    using _ileast32 = std::int_least32_t;
    using _ileast64 = std::int_least64_t;
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
    using _uleast8 = std::uint_least8_t;
    using _uleast16 = std::uint_least16_t;
    using _uleast32 = std::uint_least32_t;
    using _uleast64 = std::uint_least64_t;
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
    class ileast8;
    class ileast16;
    class ileast32;
    class ileast64;
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
    class uleast8;
    class uleast16;
    class uleast32;
    class uleast64;
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
    ATOM_ALIAS(ileast8, signed_int_wrapper<ileast8, uleast8, _ileast8, _i8>);
    ATOM_ALIAS(ileast16, signed_int_wrapper<ileast16, uleast16, _ileast16, _i16>);
    ATOM_ALIAS(ileast32, signed_int_wrapper<ileast32, uleast32, _ileast32, _i32>);
    ATOM_ALIAS(ileast64, signed_int_wrapper<ileast64, uleast64, _ileast64, _i64>);
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
    ATOM_ALIAS(uleast8, unsigned_int_wrapper<uleast8, ileast8, _uleast8, _u8>);
    ATOM_ALIAS(uleast16, unsigned_int_wrapper<uleast16, ileast16, _uleast16, _u16>);
    ATOM_ALIAS(uleast32, unsigned_int_wrapper<uleast32, ileast32, _uleast32, _u32>);
    ATOM_ALIAS(uleast64, unsigned_int_wrapper<uleast64, ileast64, _uleast64, _u64>);
    ATOM_ALIAS(ufast8, unsigned_int_wrapper<ufast8, ifast8, _ufast8, _u8>);
    ATOM_ALIAS(ufast16, unsigned_int_wrapper<ufast16, ifast16, _ufast16, _u16>);
    ATOM_ALIAS(ufast32, unsigned_int_wrapper<ufast32, ifast32, _ufast32, _u32>);
    ATOM_ALIAS(ufast64, unsigned_int_wrapper<ufast64, ifast64, _ufast64, _u64>);
    ATOM_ALIAS(umax, unsigned_int_wrapper<umax, imax, _umax>);
    ATOM_ALIAS(usize, unsigned_int_wrapper<usize, isize, _usize>);
}
