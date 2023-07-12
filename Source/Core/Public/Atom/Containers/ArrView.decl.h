#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <typename T>
    struct _ArrViewImplBase
    {
    public:
        using TElem = T;

    public:
        constexpr _ArrViewImplBase() noex = default;

        constexpr _ArrViewImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 } { }

        constexpr _ArrViewImplBase(const T* arr, usize count) noex:
            _arr{ arr }, _count{ count } { }

    public:
        constexpr auto _Data() const noex
            -> const T*
        {
            return _arr;
        }

        constexpr auto _Count() const noex
            -> usize
        {
            return _count;
        }

    protected:
        const T* _arr;
        usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrView: public _ConstArrImplHelper<_ArrViewImplBase<T>>
    {
        using Base = _ConstArrImplHelper<_ArrViewImplBase<T>>;
        using BaseImpl = _ArrViewImplBase<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView(NullPtr) noex:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrView& operator =(NullPtr) noex
        {
            *this = ArrView(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RArrRangeOf<TRange, T>
        constexpr ArrView(const TRange& range) noex:
            Base{BaseImpl{ range.Data(), range.Count() }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RArrRangeOf<TRange, T>
        constexpr ArrView& operator =(const TRange& range) noex
        {
            *this = ArrView{ range.Data(), range.Count() };
        }
    };
}