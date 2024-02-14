module;
#include "atom/preprocessors.h"

export module atom.core:core.float_wrapper;
import :core.num_wrapper;
import :core.char_wrapper;
import :std;

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_final_type, typename in_unwrapped_type>
    class _float_wrapper_impl
    {
    public:
        using final_type = in_final_type;
        using unwrapped_type = in_unwrapped_type;

    public:
        static consteval auto min() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<unwrapped_type>::min());
        }

        static consteval auto max() -> unwrapped_type
        {
            return unwrapped_type(std::numeric_limits<unwrapped_type>::max());
        }

        static consteval auto bits() -> unwrapped_type
        {
            return unwrapped_type(sizeof(unwrapped_type) * 8);
        }

        static consteval auto nan() -> unwrapped_type
        {
            return unwrapped_type();
        }

        static constexpr auto floor(unwrapped_type val) -> unwrapped_type
        {
            return std::floor(val);
        }

        static constexpr auto ceil(unwrapped_type val) -> unwrapped_type
        {
            return std::ceil(val);
        }

        static constexpr auto round(unwrapped_type val) -> unwrapped_type
        {
            return std::round(val);
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
            return true;
        }
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename impl_type>
    class float_wrapper: public num_wrapper<impl_type>
    {
        using this_type = float_wrapper<impl_type>;
        using base_type = num_wrapper<impl_type>;
        using final_type = typename base_type::final_type;

    public:
        using unwrapped_type = typename base_type::unwrapped_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto nan() -> this_type
        {
            return _wrap_final(impl_type::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> this_type
        {
            return _wrap_final(impl_type::floor(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> this_type
        {
            return _wrap_final(impl_type::ceil(_value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> this_type
        {
            return _wrap_final(impl_type::round(_value));
        }

    protected:
        using base_type::_wrap_final;

    public:
        using base_type::_value;
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
export namespace atom
{
    using _f16 = float;
    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    ATOM_ALIAS(f16, float_wrapper<_float_wrapper_impl<f16, _f16>>);
    ATOM_ALIAS(f32, float_wrapper<_float_wrapper_impl<f32, _f32>>);
    ATOM_ALIAS(f64, float_wrapper<_float_wrapper_impl<f64, _f64>>);
    ATOM_ALIAS(f128, float_wrapper<_float_wrapper_impl<f128, _f128>>);
}
