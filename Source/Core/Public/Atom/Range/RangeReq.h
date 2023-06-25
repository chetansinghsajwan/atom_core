#pragma once
#include "IterReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept _RRange = requires(const TRange& range)
    {
        typename TRange::TElem;
        typename TRange::TIter;
        typename TRange::TIterEnd;

        requires RSameAs<typename TRange::TElem, typename TRange::TIter::TElem>;

        { range.Iter() } -> RSameAs<typename TRange::TIter>;
        { range.IterEnd() } -> RSameAs<typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept _RRangeOf = requires(const TRange& range)
    {
        requires _RRange<TRange>;
        requires RConvertibleTo<typename TRange::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept _RMutRange = requires(TRange& range)
    {
        requires _RRange<TRange>;

        typename TRange::TMutIter;
        typename TRange::TMutIterEnd;

        requires RConvertibleTo<typename TRange::TMutIter, typename TRange::TIter>;

        { range.MutIter() } -> RSameAs<typename TRange::TMutIter>;
        { range.MutIterEnd() } -> RSameAs<typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept _RMutRangeOf = requires(TRange& range)
    {
        requires _RMutRange<TRange>;
        requires RConvertibleTo<typename TRange::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {Range}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RRange = requires
    {
        requires _RRange<TRange>;
        requires RIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {Range} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutRange = requires
    {
        requires _RMutRange<TRange>;
        requires RIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {FwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RFwdRange = requires
    {
        requires _RRange<TRange>;
        requires RFwdIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {FwdRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RFwdIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutFwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutFwdRange = requires
    {
        requires _RMutRange<TRange>;
        requires RFwdIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutFwdIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutFwdRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutFwdRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RFwdIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutFwdIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {BidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RBidiRange = requires
    {
        requires _RRange<TRange>;
        requires RBidiIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {BidiRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBidiRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RBidiIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutBidiRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutBidiRange = requires
    {
        requires _RMutRange<TRange>;
        requires RBidiIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutBidiIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutBidiRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutBidiRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RBidiIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutBidiIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {JumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RJumpRange = requires
    {
        requires _RRange<TRange>;
        requires RJumpIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {JumpRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RJumpRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RJumpIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutJumpRange = requires
    {
        requires _RMutRange<TRange>;
        requires RJumpIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutJumpIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutJumpRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutJumpRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RJumpIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutJumpIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {ArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RArrRange = requires
    {
        requires _RRange<TRange>;
        requires RArrIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {ArrRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RArrIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutArrRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutArrRange = requires
    {
        requires _RMutRange<TRange>;
        requires RArrIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutArrIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutArrRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutArrRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RArrIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutArrIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {RevRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename>
    concept RRevRange = requires
    {
        requires RBidiRange<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {RevRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRevRangeOf = requires
    {
        requires RBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRevRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutRevRange = requires
    {
        requires RMutBidiRangeOf<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutRevRange} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutRevRangeOf = requires
    {
        requires RMutBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {CommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RCommonRange = requires
    {
        requires _RRange<TRange>;
        requires RSameAs<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TRange} is {MutCommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutCommonRange = requires
    {
        requires RCommonRange<TRange>;
        requires RSameAs<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };
}