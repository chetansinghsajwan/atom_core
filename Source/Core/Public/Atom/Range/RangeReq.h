#pragma once
#include "IterReq.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept _RRangeBase = requires(const TRange& range)
    {
        typename TRange::TElem;
        typename TRange::TIter;
        typename TRange::TIterEnd;

        { range.Iter() } -> RSameAs<typename TRange::TIter>;
        { range.IterEnd() } -> RSameAs<typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept _RMutRangeBase = requires(TRange& range)
    {
        typename TRange::TElem;
        typename TRange::TMutIter;
        typename TRange::TMutIterEnd;

        { range.MutIter() } -> RSameAs<typename TRange::TMutIter>;
        { range.MutIterEnd() } -> RSameAs<typename TRange::TMutIterEnd>;

        requires RConvertibleTo<typename TRange::TMutIter, typename TRange::TIter>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {Range}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RIter<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {FwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RFwdIter<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutFwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutFwdRange = requires
    {
        requires _RMutRangeBase<TRange, T>;
        requires RFwdRange<TRange, T>;
        requires RMutFwdIter<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {BidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBidiRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RBidiIter<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutBidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutBidiRange = requires
    {
        requires _RMutRangeBase<TRange, T>;
        requires RBidiRange<TRange, T>;
        requires RMutBidiIter<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {JumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RJumpRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RJumpIter<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutJumpRange = requires
    {
        requires _RMutRangeBase<TRange, T>;
        requires RJumpRange<TRange, T>;
        requires RMutJumpIter<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {ArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RArrIter<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutArrRange = requires
    {
        requires _RMutRangeBase<TRange, T>;
        requires RArrRange<TRange, T>;
        requires RMutArrIter<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {RevRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRevRange = requires
    {
        requires RBidiRange<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRevRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutRevRange = requires
    {
        requires RMutBidiRange<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {CommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RCommonRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RSameAs<typename TRange::TIter, typename TRange::TIter>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutCommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutCommonRange = requires
    {
        requires _RRangeBase<TRange, T>;
        requires RSameAs<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Refactor this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRange<TTI::TRemoveCVRef<TRange>, typename TTI::TRemoveCVRef<TRange>::TElem>
    constexpr auto begin(const TRange& range) noexcept
    {
        // if constexpr (!RConst<TRange> && RMutFwdRange<TTI::TRemoveCVRef<TRange>, 
        //     typename TTI::TRemoveCVRef<TRange>::TElem>)
        // {
        //     return range.MutIter();
        // }
        // else
        {
            return range.Iter();
        }
    }

    template <typename TRange>
    requires RMutFwdRange<TRange, typename TRange::TElem>
    constexpr auto begin(TRange& range) noexcept
    {
        return range.MutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Refactor this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRange<TTI::TRemoveCVRef<TRange>, typename TTI::TRemoveCVRef<TRange>::TElem>
    constexpr auto end(const TRange& range) noexcept
    {
        // if constexpr (!RConst<TRange> && RMutFwdRange<TTI::TRemoveCVRef<TRange>, 
        //     typename TTI::TRemoveCVRef<TRange>::TElem>)
        // {
        //     return range.MutIterEnd();
        // }
        // else
        {
            return range.IterEnd();
        }
    }

    template <typename TRange>
    requires RMutFwdRange<TRange, typename TRange::TElem>
    constexpr auto end(TRange&& range) noexcept
    {
        return range.MutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T1Range, typename T2Range>
    requires 
        RRange<T1Range, typename T1Range::TElem> &&
        RRange<T2Range, typename T2Range::TElem>
    constexpr bool operator ==(const T1Range& range1, const T2Range& range2) noexcept
    {
        return _Compare(range1, range2) == 0;
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T1Range, typename T2Range>
    requires 
        RRange<T1Range, typename T1Range::TElem> &&
        RRange<T2Range, typename T2Range::TElem>
    constexpr bool operator !=(const T1Range& range1, const T2Range& range2) noexcept
    {
        return !(range1 == range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename T1Range, typename T2Range>
    constexpr int8 _Compare(const T1Range& range1, const T2Range& range2) noexcept
    {
        // TODO: Implement this.
        return 0;
    }
}