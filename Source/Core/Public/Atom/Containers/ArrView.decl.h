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
    private:
        using Base = _ConstArrImplHelper<_ArrViewImplBase<T>>;
        using BaseImpl = _ArrViewImplBase<T>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefCtor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ArrView() noex = default;

        /// ----------------------------------------------------------------------------------------
        /// NullCtor.
        /// ----------------------------------------------------------------------------------------
        cexpr ctor ArrView(NullPtr) noex:
            Base{BaseImpl{ nullptr }} { }

        /// ----------------------------------------------------------------------------------------
        /// NullOper.
        /// ----------------------------------------------------------------------------------------
        cexpr fn operator =(NullPtr) noex -> ArrView&
        {
            *this = ArrView(nullptr);
        }

        /// ----------------------------------------------------------------------------------------
        /// ParamCtor.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RArrRangeOf<TRange, T>
        cexpr ctor ArrView(const TRange& range) noex:
            Base{BaseImpl{ range.Data(), range.Count() }} { }

        /// ----------------------------------------------------------------------------------------
        /// ParamOper.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RArrRangeOf<TRange, T>
        cexpr fn operator =(const TRange& range) noex -> ArrView&
        {
            *this = ArrView{ range.Data(), range.Count() };
        }
    };
}