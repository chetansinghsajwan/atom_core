module;
#include "atom/preprocessors.h"

export module atom.core:core.int_wrapper;
import :core.char_wrapper;
import :core.num_wrapper;
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

    template <typename final_type, typename unwrapped_type, typename limit_type = unwrapped_type>
    class int_wrapper
        : public num_wrapper<_num_impl<final_type, unwrapped_type, limit_type>>
        , public _int_id
    {
        using base_type = num_wrapper<_num_impl<final_type, unwrapped_type, limit_type>>;

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

    ATOM_ALIAS(i8, int_wrapper<i8, _i8>);
    ATOM_ALIAS(i16, int_wrapper<i16, _i16>);
    ATOM_ALIAS(i32, int_wrapper<i32, _i32>);
    ATOM_ALIAS(i64, int_wrapper<i64, _i64>);
    ATOM_ALIAS(u8, int_wrapper<u8, _u8>);
    ATOM_ALIAS(u16, int_wrapper<u16, _u16>);
    ATOM_ALIAS(u32, int_wrapper<u32, _u32>);
    ATOM_ALIAS(u64, int_wrapper<u64, _u64>);
    ATOM_ALIAS(ileast8, int_wrapper<ileast8, _ileast8, _i8>);
    ATOM_ALIAS(ileast16, int_wrapper<ileast16, _ileast16, _i16>);
    ATOM_ALIAS(ileast32, int_wrapper<ileast32, _ileast32, _i32>);
    ATOM_ALIAS(ileast64, int_wrapper<ileast64, _ileast64, _i64>);
    ATOM_ALIAS(uleast8, int_wrapper<uleast8, _uleast8, _u8>);
    ATOM_ALIAS(uleast16, int_wrapper<uleast16, _uleast16, _u16>);
    ATOM_ALIAS(uleast32, int_wrapper<uleast32, _uleast32, _u32>);
    ATOM_ALIAS(uleast64, int_wrapper<uleast64, _uleast64, _u64>);
    ATOM_ALIAS(ifast8, int_wrapper<ifast8, _ifast8, _i8>);
    ATOM_ALIAS(ifast16, int_wrapper<ifast16, _ifast16, _i16>);
    ATOM_ALIAS(ifast32, int_wrapper<ifast32, _ifast32, _i32>);
    ATOM_ALIAS(ifast64, int_wrapper<ifast64, _ifast64, _i64>);
    ATOM_ALIAS(ufast8, int_wrapper<ufast8, _ufast8, _u8>);
    ATOM_ALIAS(ufast16, int_wrapper<ufast16, _ufast16, _u16>);
    ATOM_ALIAS(ufast32, int_wrapper<ufast32, _ufast32, _u32>);
    ATOM_ALIAS(ufast64, int_wrapper<ufast64, _ufast64, _u64>);
    ATOM_ALIAS(imax, int_wrapper<imax, _imax>);
    ATOM_ALIAS(umax, int_wrapper<umax, _umax>);
    ATOM_ALIAS(isize, int_wrapper<isize, _isize>);
    ATOM_ALIAS(usize, int_wrapper<usize, _usize>);
}
