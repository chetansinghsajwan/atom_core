export module atom.core:core.float_wrapper;

import std;
import :core.num_wrapper;

#include "atom/core/preprocessors.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_final_t, typename in_unwrapped_t>
    class _float_wrapper_impl
    {
    public:
        using final_t = in_final_t;
        using unwrapped_t = in_unwrapped_t;

    public:
        static consteval auto min() -> unwrapped_t
        {
            return unwrapped_t(std::numeric_limits<unwrapped_t>::min());
        }

        static consteval auto max() -> unwrapped_t
        {
            return unwrapped_t(std::numeric_limits<unwrapped_t>::max());
        }

        static consteval auto bits() -> unwrapped_t
        {
            return unwrapped_t(sizeof(unwrapped_t) * 8);
        }

        static consteval auto nan() -> unwrapped_t
        {
            return unwrapped_t();
        }

        static constexpr auto floor(unwrapped_t val) -> unwrapped_t
        {
            return std::floor(val);
        }

        static constexpr auto ceil(unwrapped_t val) -> unwrapped_t
        {
            return std::ceil(val);
        }

        static constexpr auto round(unwrapped_t val) -> unwrapped_t
        {
            return std::round(val);
        }

        static constexpr auto is_add_safe(unwrapped_t lhs, unwrapped_t rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_sub_safe(unwrapped_t lhs, unwrapped_t rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_mul_safe(unwrapped_t lhs, unwrapped_t rhs) -> bool
        {
            return true;
        }

        static constexpr auto is_div_safe(unwrapped_t lhs, unwrapped_t rhs) -> bool
        {
            return true;
        }

        template <typename num_t>
        static constexpr auto is_conversion_safe_from(num_t num) -> bool
        {
            return is_conversion_safe_from_unwrapped<typename num_t::unwrapped_t>(num);
        }

        template <typename num_t>
        static constexpr auto is_conversion_safe_from_unwrapped(num_t num) -> bool
        {
            return true;
        }

        template <typename num_t>
        static constexpr auto is_conversion_safe_to(num_t num) -> bool
        {
            return true;
        }

        template <typename num_t>
        static constexpr auto is_conversion_safe_to_unwrapped(num_t num) -> bool
        {
            return true;
        }
    };
}

namespace atom
{
    export using _f16 = float;
    export using _f32 = float;
    export using _f64 = double;
    export using _f128 = long double;

    // export ATOM_ALIAS(f16, num_wrapper<_float_wrapper_impl<f16, _f16>>);
    // export ATOM_ALIAS(f32, num_wrapper<_float_wrapper_impl<f32, _f32>>);
    // export ATOM_ALIAS(f64, num_wrapper<_float_wrapper_impl<f64, _f64>>);
    // export ATOM_ALIAS(f128, num_wrapper<_float_wrapper_impl<f128, _f128>>);

    export using f16 = _f16;
    export using f32 = _f32;
    export using f64 = _f64;
    export using f128 = _f128;
}
