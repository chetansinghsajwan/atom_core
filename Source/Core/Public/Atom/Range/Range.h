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
    class Range;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname _TIter, tname _TIterEnd>
    requires(RIterPair<_TIter, _TIterEnd>)
    class Range<_TIter, _TIterEnd>
    {
        pub using TElem = tname _TIter::TElem;
        pub using TMutIter = _TIter;
        pub using TMutIterEnd = _TIterEnd;
        pub using TIter = _TIter;
        pub using TIterEnd = _TIterEnd;

        pub cexpr ctor Range(TIter iter, TIterEnd end) noex:
            _iter{ iter }, _end{ end } { }

        pub cexpr fn iter() const noex -> TIter
            requires(RIterPair<TIter, TIterEnd>)
        {
            return TIter{ _iter };
        }

        pub cexpr fn iterEnd() const noex -> TIterEnd
            requires(RIterPair<TIter, TIterEnd>)
        {
            return TIterEnd{ _end };
        }

        pub cexpr fn mutIter() noex -> TMutIter
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _iter;
        }

        pub cexpr fn mutIterEnd() noex -> TMutIterEnd
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _end;
        }

        pub cexpr fn count() const noex -> usize
            requires(RJumpIterPair<TIter, TIterEnd>)
        {
            return _end - _iter;
        }

        pub cexpr fn data() const noex -> const TElem*
            requires(RArrIterPair<TIter, TIterEnd>)
        {
            return &*_iter;
        }

        pub cexpr fn data() noex -> TElem*
            requires(RMutArrIterPair<TIter, TIterEnd>)
        {
            return &*_iter;
        }

        prot TIter _iter;
        prot TIterEnd _end;
    };

    template <tname TIter, tname TIterEnd>
    requires(RIterPair<TIter, TIterEnd>)
    Range(TIter iter, TIterEnd end) -> Range<TIter, TIterEnd>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class Range<const T*, const T*> extends Range<ArrIter<T>, ArrIter<T>>
    {
        pub using Base = Range<ArrIter<T>, ArrIter<T>>;

        pub cexpr ctor Range(const T* arr, const T* end) noex:
            Base{ arr, end } { }

        pub cexpr ctor Range(const T* arr, usize count) noex:
            Base{ arr, arr + count } { }

        pub template <usize count>
        cexpr ctor Range(const T(&arr)[count]) noex:
            Base{ arr, arr + count } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class Range<T*, T*> extends Range<MutArrIter<T>, MutArrIter<T>>
    {
        pub using Base = Range<MutArrIter<T>, MutArrIter<T>>;

        pub cexpr ctor Range(T* arr, T* end) noex:
            Base{ arr, end } { }

        pub cexpr ctor Range(T* arr, usize count) noex:
            Base{ arr, arr + count } { }

        pub template <usize count>
        cexpr ctor Range(T(&arr)[count]) noex:
            Base{ arr, arr + count } { }
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
    class Range<InitList<T>> extends Range<const T*, const T*>
    {
        pub using Base = Range<const T*, const T*>;

        pub cexpr ctor Range(const InitList<T>& init) noex:
            Base{ init.begin(), init.end() } { }
    };

    template <tname T>
    Range(const InitList<T>& init) -> Range<InitList<T>>;
}