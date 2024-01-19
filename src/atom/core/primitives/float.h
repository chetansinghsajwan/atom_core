#pragma once
#include "num.h"

namespace atom
{
    template <std::size_t size>
    class float_string;

    template <typename self_type, typename value_type>
    class _float_impl: public _num_impl<self_type, value_type, value_type>
    {
        using base_type = _num_impl<self_type, value_type, value_type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # to do
        ///
        /// - review this. [`float_string`] has different sting requirements.
        /// ----------------------------------------------------------------------------------------
        using string_type = float_string<base_type::max_digits_count() + 1>;

    public:
        static consteval auto nan() -> value_type
        {
            return value_type();
        }

        static constexpr auto floor(value_type val) -> value_type
        {
            return std::floor(val);
        }

        static constexpr auto ceil(value_type val) -> value_type
        {
            return std::ceil(val);
        }

        static constexpr auto round(value_type val) -> value_type
        {
            return std::round(val);
        }

        static constexpr auto to_string(value_type val) -> string_type;

        template <typename tout>
        static constexpr auto to_string_out(value_type val, tout&& out) -> tout
        {
            string_type str = to_string();
            out += str;
            return out;
        }

    private:
        static constexpr auto _to_string(value_type val, uchar* str) -> uchar*
        {
            return str;
        }
    };

    template <typename impl_type>
    class _float: public num<impl_type>
    {
        using base_type = num<impl_type>;
        using self = _float<impl_type>;

    public:
        using self_type = typename base_type::self_type;
        using value_type = typename base_type::value_type;
        using string_type = typename base_type::string_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto eq_zero_approx() const -> bool
        {
            return _val == 0;
            ;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// utils
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto nan() -> self
        {
            return _make(impl_type::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> self
        {
            return _make(impl_type::floor(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> self
        {
            return _make(impl_type::ceil(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> self
        {
            return _make(impl_type::round(_val));
        }

    protected:
        using base_type::_make;

    public:
        using base_type::_val;
    };

    using _f16 = float;
    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    class f16: public _float<_float_impl<f16, _f16>>
    {
        using base_type = _float<_float_impl<f16, _f16>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class f32: public _float<_float_impl<f32, _f32>>
    {
        using base_type = _float<_float_impl<f32, _f32>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class f64: public _float<_float_impl<f64, _f64>>
    {
        using base_type = _float<_float_impl<f64, _f64>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class f128: public _float<_float_impl<f128, _f128>>
    {
        using base_type = _float<_float_impl<f128, _f128>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
};
