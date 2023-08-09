#pragma once
#include "IterReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept _RRange = requires(const TRange& range)
    {
        tname TRange::TElem;
        tname TRange::TIter;
        tname TRange::TIterEnd;

        requires RSameAs<tname TRange::TElem, tname TRange::TIter::TElem>;

        { range.iter() } -> RSameAs<tname TRange::TIter>;
        { range.iterEnd() } -> RSameAs<tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept _RRangeOf = requires(const TRange& range)
    {
        requires _RRange<TRange>;
        requires RConvertibleTo<tname TRange::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept _RMutRange = requires(TRange& range)
    {
        requires _RRange<TRange>;

        tname TRange::TMutIter;
        tname TRange::TMutIterEnd;

        requires RConvertibleTo<tname TRange::TMutIter, tname TRange::TIter>;

        { range.mutIter() } -> RSameAs<tname TRange::TMutIter>;
        { range.mutIterEnd() } -> RSameAs<tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept _RMutRangeOf = requires(TRange& range)
    {
        requires _RMutRange<TRange>;
        requires RConvertibleTo<tname TRange::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {Range}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RRange = requires
    {
        requires _RRange<TRange>;
        requires RIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {Range} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutRange = requires
    {
        requires _RMutRange<TRange>;
        requires RIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
        requires RMutIterPair<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
        requires RMutIterPairOf<tname TRange::TMutIter, tname TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {FwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RFwdRange = requires
    {
        requires _RRange<TRange>;
        requires RFwdIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {FwdRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RFwdRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RFwdIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutFwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutFwdRange = requires
    {
        requires _RMutRange<TRange>;
        requires RFwdIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
        requires RMutFwdIterPair<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutFwdRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutFwdRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RFwdIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
        requires RMutFwdIterPairOf<tname TRange::TMutIter, tname TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {BidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RBidiRange = requires
    {
        requires _RRange<TRange>;
        requires RBidiIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {BidiRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RBidiRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RBidiIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutBidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutBidiRange = requires
    {
        requires _RMutRange<TRange>;
        requires RBidiIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
        requires RMutBidiIterPair<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutBidiRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutBidiRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RBidiIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
        requires RMutBidiIterPairOf<tname TRange::TMutIter, tname TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {JumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RJumpRange = requires
    {
        requires _RRange<TRange>;
        requires RJumpIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {JumpRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RJumpRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RJumpIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutJumpRange = requires
    {
        requires _RMutRange<TRange>;
        requires RJumpIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
        requires RMutJumpIterPair<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutJumpRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutJumpRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RJumpIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
        requires RMutJumpIterPairOf<tname TRange::TMutIter, tname TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {ArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RArrRange = requires
    {
        requires _RRange<TRange>;
        requires RArrIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {ArrRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RArrRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RArrIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutArrRange = requires
    {
        requires _RMutRange<TRange>;
        requires RArrIterPair<tname TRange::TIter, tname TRange::TIterEnd>;
        requires RMutArrIterPair<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutArrRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutArrRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RArrIterPairOf<tname TRange::TIter, tname TRange::TIterEnd, T>;
        requires RMutArrIterPairOf<tname TRange::TMutIter, tname TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {RevRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname>
    concept RRevRange = requires
    {
        requires RBidiRange<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {RevRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RRevRangeOf = requires
    {
        requires RBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRevRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutRevRange = requires
    {
        requires RMutBidiRange<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRevRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange, tname T>
    concept RMutRevRangeOf = requires
    {
        requires RMutBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {CommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RCommonRange = requires
    {
        requires _RRange<TRange>;
        requires RSameAs<tname TRange::TIter, tname TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutCommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    concept RMutCommonRange = requires
    {
        requires RCommonRange<TRange>;
        requires RSameAs<tname TRange::TMutIter, tname TRange::TMutIterEnd>;
    };
}