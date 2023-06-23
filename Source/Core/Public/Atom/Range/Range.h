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
    struct Range;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd>
    requires RIterPairOf<_TIter, _TIterEnd, typename _TIter::TElem>
    struct Range<_TIter, _TIterEnd>
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// @TODO: Refactor this.
        /// ----------------------------------------------------------------------------------------
        using TElem = typename _TIter::TElem;
        using TMutIter = _TIter;
        using TMutIterEnd = _TIterEnd;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;

    public:
        constexpr Range(TIter iter, TIterEnd end) noexcept:
            _iter{ iter }, _end{ end } { }

    public:
        constexpr TIter Iter() const noexcept
        requires RIterPairOf<TIter, TIterEnd, TElem>
        {
            return TIter{ _iter };
        }

        constexpr TIterEnd IterEnd() const noexcept
        requires RIterPairOf<TIter, TIterEnd, TElem>
        {
            return TIterEnd{ _end };
        }

        constexpr TMutIter MutIter() noexcept
        requires RMutFwdIterPairOf<TMutIter, TMutIterEnd, TElem>
        {
            return _iter;
        }

        constexpr TMutIterEnd MutIterEnd() noexcept
        requires RMutFwdIterPairOf<TMutIter, TMutIterEnd, TElem>
        {
            return _end;
        }

        constexpr usize Count() const noexcept
        requires RJumpIterPairOf<TIter, TIterEnd, TElem>
        {
            return _end - _iter;
        }

        constexpr const TElem* Data() const noexcept
        requires RArrIterPairOf<TIter, TIterEnd, TElem>
        {
            return &*_iter;
        }

        constexpr TElem* Data() noexcept
        requires RMutArrIterOf<TMutIter, TMutIterEnd, TElem>
        {
            return &*_iter;
        }

    protected:
        TIter _iter;
        TIterEnd _end;
    };

    template <typename TIter, typename TIterEnd>
    requires RIterPairOf<TIter, TIterEnd, typename TIter::TElem>
    Range(TIter iter, TIterEnd end) -> Range<TIter, TIterEnd>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct Range<const T*, const T*>: Range<ArrIter<T>, ArrIter<T>>
    {
        using _TBase = Range<ArrIter<T>, ArrIter<T>>;

        constexpr Range(const T* arr, const T* end) noexcept:
            _TBase{ arr, end } { }

        constexpr Range(const T* arr, usize count) noexcept:
            _TBase{ arr, arr + count } { }

        template <usize count>
        constexpr Range(const T(&arr)[count]) noexcept:
            _TBase{ arr, arr + count } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct Range<T*, T*>: Range<MutArrIter<T>, MutArrIter<T>>
    {
        using _TBase = Range<MutArrIter<T>, MutArrIter<T>>;

        constexpr Range(T* arr, T* end) noexcept:
            _TBase{ arr, end } { }

        constexpr Range(T* arr, usize count) noexcept:
            _TBase{ arr, arr + count } { }

        template <usize count>
        constexpr Range(T(&arr)[count]) noexcept:
            _TBase{ arr, arr + count } { }
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
    struct Range<InitList<T>>: Range<const T*, const T*>
    {
        using Base = Range<const T*, const T*>;

        constexpr Range(const InitList<T>& init) noexcept:
            Base{ init.begin(), init.end() } { }
    };

    template <typename T>
    Range(const InitList<T>& init) -> Range<InitList<T>>;
}