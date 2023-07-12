#pragma once
#include "RangeReq.h"
#include "ArrIter.h"
#include "InitList.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname... TArgs>
    struct Range;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname _TIter, tname _TIterEnd>
    requires RIterPair<_TIter, _TIterEnd>
    struct Range<_TIter, _TIterEnd>
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// @TODO: Refactor this.
        /// ----------------------------------------------------------------------------------------
        using TElem = tname _TIter::TElem;
        using TMutIter = _TIter;
        using TMutIterEnd = _TIterEnd;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;

    public:
        constexpr Range(TIter iter, TIterEnd end) noex:
            _iter{ iter }, _end{ end } { }

    public:
        constexpr TIter Iter() const noex
        requires RIterPair<TIter, TIterEnd>
        {
            return TIter{ _iter };
        }

        constexpr TIterEnd IterEnd() const noex
        requires RIterPair<TIter, TIterEnd>
        {
            return TIterEnd{ _end };
        }

        constexpr TMutIter MutIter() noex
        requires RMutIterPair<TMutIter, TMutIterEnd>
        {
            return _iter;
        }

        constexpr TMutIterEnd MutIterEnd() noex
        requires RMutIterPair<TMutIter, TMutIterEnd>
        {
            return _end;
        }

        constexpr usize Count() const noex
        requires RJumpIterPair<TIter, TIterEnd>
        {
            return _end - _iter;
        }

        constexpr const TElem* Data() const noex
        requires RArrIterPair<TIter, TIterEnd>
        {
            return &*_iter;
        }

        constexpr TElem* Data() noex
        requires RMutArrIterPair<TIter, TIterEnd>
        {
            return &*_iter;
        }

    protected:
        TIter _iter;
        TIterEnd _end;
    };

    template <tname TIter, tname TIterEnd>
    requires RIterPair<TIter, TIterEnd>
    Range(TIter iter, TIterEnd end) -> Range<TIter, TIterEnd>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct Range<const T*, const T*>: Range<ArrIter<T>, ArrIter<T>>
    {
        using _TBase = Range<ArrIter<T>, ArrIter<T>>;

        constexpr Range(const T* arr, const T* end) noex:
            _TBase{ arr, end } { }

        constexpr Range(const T* arr, usize count) noex:
            _TBase{ arr, arr + count } { }

        template <usize count>
        constexpr Range(const T(&arr)[count]) noex:
            _TBase{ arr, arr + count } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct Range<T*, T*>: Range<MutArrIter<T>, MutArrIter<T>>
    {
        using _TBase = Range<MutArrIter<T>, MutArrIter<T>>;

        constexpr Range(T* arr, T* end) noex:
            _TBase{ arr, end } { }

        constexpr Range(T* arr, usize count) noex:
            _TBase{ arr, arr + count } { }

        template <usize count>
        constexpr Range(T(&arr)[count]) noex:
            _TBase{ arr, arr + count } { }
    };

    template <tname T>
    Range(T* iter, T* end) -> Range<T*, T*>;

    template <tname T>
    Range(T* iter, usize count) -> Range<T*, T*>;

    template <tname T, usize count>
    Range(T(&arr)[count]) -> Range<T*, T*>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct Range<InitList<T>>: Range<const T*, const T*>
    {
        using Base = Range<const T*, const T*>;

        constexpr Range(const InitList<T>& init) noex:
            Base{ init.begin(), init.end() } { }
    };

    template <tname T>
    Range(const InitList<T>& init) -> Range<InitList<T>>;
}