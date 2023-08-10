#pragma once
#include "RangeReq.h"
#include "ArrIter.h"
#include "InitList.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename... TArgs>
    class Range;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd>
    requires(RIterPair<_TIter, _TIterEnd>)
    class Range<_TIter, _TIterEnd>
    {
        pub using TElem = typename _TIter::TElem;
        pub using TMutIter = _TIter;
        pub using TMutIterEnd = _TIterEnd;
        pub using TIter = _TIter;
        pub using TIterEnd = _TIterEnd;

        pub constexpr ctor Range(TIter iter, TIterEnd end):
            _iter{ iter }, _end{ end } { }

        pub constexpr fn iter() const -> TIter
            requires(RIterPair<TIter, TIterEnd>)
        {
            return TIter{ _iter };
        }

        pub constexpr fn iterEnd() const -> TIterEnd
            requires(RIterPair<TIter, TIterEnd>)
        {
            return TIterEnd{ _end };
        }

        pub constexpr fn mutIter() -> TMutIter
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _iter;
        }

        pub constexpr fn mutIterEnd() -> TMutIterEnd
            requires(RMutIterPair<TMutIter, TMutIterEnd>)
        {
            return _end;
        }

        pub constexpr fn count() const -> usize
            requires(RJumpIterPair<TIter, TIterEnd>)
        {
            return _end - _iter;
        }

        pub constexpr fn data() const -> const TElem*
            requires(RArrIterPair<TIter, TIterEnd>)
        {
            return &*_iter;
        }

        pub constexpr fn data() -> TElem*
            requires(RMutArrIterPair<TIter, TIterEnd>)
        {
            return &*_iter;
        }

        prot TIter _iter;
        prot TIterEnd _end;
    };

    template <typename TIter, typename TIterEnd>
    requires(RIterPair<TIter, TIterEnd>)
    Range(TIter iter, TIterEnd end) -> Range<TIter, TIterEnd>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<const T*, const T*> extends Range<ArrIter<T>, ArrIter<T>>
    {
        pub using Base = Range<ArrIter<T>, ArrIter<T>>;

        pub constexpr ctor Range(const T* arr, const T* end):
            Base{ arr, end } { }

        pub constexpr ctor Range(const T* arr, usize count):
            Base{ arr, arr + count } { }

        pub template <usize count>
        constexpr ctor Range(const T(&arr)[count]):
            Base{ arr, arr + count } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<T*, T*> extends Range<MutArrIter<T>, MutArrIter<T>>
    {
        pub using Base = Range<MutArrIter<T>, MutArrIter<T>>;

        pub constexpr ctor Range(T* arr, T* end):
            Base{ arr, end } { }

        pub constexpr ctor Range(T* arr, usize count):
            Base{ arr, arr + count } { }

        pub template <usize count>
        constexpr ctor Range(T(&arr)[count]):
            Base{ arr, arr + count } { }
    };

    template <typename T>
    Range(T* iter, T* end) -> Range<T*, T*>;

    template <typename T>
    Range(T* iter, usize count) -> Range<T*, T*>;

    template <typename T, usize count>
    Range(T(&arr)[count]) -> Range<T*, T*>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class Range<InitList<T>> extends Range<const T*, const T*>
    {
        pub using Base = Range<const T*, const T*>;

        pub constexpr ctor Range(const InitList<T>& init):
            Base{ init.begin(), init.end() } { }
    };

    template <typename T>
    Range(const InitList<T>& init) -> Range<InitList<T>>;
}