export module atom.core:core.float_wrapper;
import :core.num_wrapper;
import :core.char_wrapper;
import :std;

namespace atom
{
    export extern "C++" template <std::size_t size>
    class float_string;

    template <typename this_final_type, typename value_type>
    class _float_impl: public _num_impl<this_final_type, value_type, value_type>
    {
        using base_type = _num_impl<this_final_type, value_type, value_type>;

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
    };

    template <typename impl_type>
    class _float: public num<impl_type>
    {
        using base_type = num<impl_type>;
        using this_type = _float<impl_type>;

    public:
        using this_final_type = typename base_type::this_final_type;
        using value_type = typename base_type::value_type;

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
        static consteval auto nan() -> this_type
        {
            return _make(impl_type::nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> this_type
        {
            return _make(impl_type::floor(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> this_type
        {
            return _make(impl_type::ceil(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> this_type
        {
            return _make(impl_type::round(_val));
        }

    protected:
        using base_type::_make;

    public:
        using base_type::_val;
    };
}

export namespace atom
{
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
