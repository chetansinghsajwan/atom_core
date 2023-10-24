#pragma once
#include "Num.h"

namespace Atom
{
    template <size_t size>
    class FloatStr;

    template <typename TSelf, typename TVal>
    class _FloatImpl: public _NumImpl<TSelf, TVal, TVal>
    {
        using Base = _NumImpl<TSelf, TVal, TVal>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # To Do
        /// 
        /// - Review this. [`FloatStr`] has different sting requirements.
        /// ----------------------------------------------------------------------------------------
        using TStr = FloatStr<Base::MaxDigitsCount() + 1>;

    public:
        static consteval auto Nan() -> TVal
        {
            return TVal();
        }

        static constexpr auto Floor(TVal val) -> TVal
        {
            return std::floor(val);
        }

        static constexpr auto Ceil(TVal val) -> TVal
        {
            return std::ceil(val);
        }

        static constexpr auto Round(TVal val) -> TVal
        {
            return std::round(val);
        }

        static constexpr auto ToStr(TVal val) -> TStr;

        template <typename TOut>
        static constexpr auto ToStrOut(TVal val, TOut&& out) -> TOut
        {
            TStr str = ToStr();
            out += str;
            return out;
        }

    private:
        static constexpr auto _ToStr(TVal val, char* str) -> char*
        {
            return str;
        }
    };

    template <typename TImpl>
    class Float: public Num<TImpl>
    {
        using Base = Num<TImpl>;
        using Self = Float<TImpl>;

    public:
        using TSelf = typename Base::TSelf;
        using TVal = typename Base::TVal;
        using TStr = typename Base::TStr;

    public:
        using Base::Base;
        using Base::operator=;

    public:

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto eqZeroApprox() const -> bool
        {
            return _val == 0;;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Utils
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static consteval auto Nan() -> Self
        {
            return _Make(TImpl::Nan());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto floor() const -> Self
        {
            return _Make(TImpl::Floor(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto ceil() const -> Self
        {
            return _Make(TImpl::Ceil(_val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto round() const -> Self
        {
            return _Make(TImpl::Round(_val));
        }

    protected:
        using Base::_Make;

    public:
        using Base::_val;
    };

    using _f16 = float;
    using _f32 = float;
    using _f64 = double;
    using _f128 = long double;

    class f16: public Float<_FloatImpl<f16, _f16>>
    {
        using Base = Float<_FloatImpl<f16, _f16>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class f32: public Float<_FloatImpl<f32, _f32>>
    {
        using Base = Float<_FloatImpl<f32, _f32>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class f64: public Float<_FloatImpl<f64, _f64>>
    {
        using Base = Float<_FloatImpl<f64, _f64>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    class f128: public Float<_FloatImpl<f128, _f128>>
    {
        using Base = Float<_FloatImpl<f128, _f128>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
};