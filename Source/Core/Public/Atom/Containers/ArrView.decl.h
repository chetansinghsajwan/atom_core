#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <tname T>
    class _ArrViewImplBase
    {
        pub using TElem = T;

        pub cexpr ctor _ArrViewImplBase() noex = default;

        pub cexpr ctor _ArrViewImplBase(NullPtr) noex:
            _arr{ nullptr }, _count{ 0 } { }

        pub cexpr ctor _ArrViewImplBase(const T* arr, usize count) noex:
            _arr{ arr }, _count{ count } { }

        pub cexpr fn _Data() const noex -> const T*
        {
            return _arr;
        }

        pub cexpr fn _Count() const noex -> usize
        {
            return _count;
        }

        prot const T* _arr;
        prot usize _count;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class ArrView extends _ConstArrImplHelper<_ArrViewImplBase<T>>
    {
        priv using Base = _ConstArrImplHelper<_ArrViewImplBase<T>>;
        priv using BaseImpl = _ArrViewImplBase<T>;

        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor ArrView() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        pub cexpr ctor ArrView(NullPtr) noex:
            Base{ BaseImpl{ nullptr } } { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn op=(NullPtr) noex -> ArrView&
        {
            *this = ArrView(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TRange>
        requires RArrRangeOf<TRange, T>
        cexpr ctor ArrView(const TRange& range) noex:
            Base{ BaseImpl{ range.Data(), range.Count() } } { }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper.
        /// ----------------------------------------------------------------------------------------
        pub template <tname TRange>
        requires RArrRangeOf<TRange, T>
        cexpr fn op=(const TRange& range) noex -> ArrView&
        {
            *this = ArrView{ range.Data(), range.Count() };
        }
    };
}