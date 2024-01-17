#pragma once
#include "IterReq.h"

// clang-format off
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

        { range.iter() } -> RSameAs<typename TRange::TIter>;
        { range.iterEnd() } -> RSameAs<typename TRange::TIterEnd>;
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

        { range.mutIter() } -> RSameAs<typename TRange::TMutIter>;
        { range.mutIterEnd() } -> RSameAs<typename TRange::TMutIterEnd>;
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
    /// Ensures `TRange` is `Range`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RRange = requires
    {
        requires _RRange<TRange>;
        requires RIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `Range` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutRange = requires
    {
        requires _RMutRange<TRange>;
        requires RIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `FwdRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RFwdRange = requires
    {
        requires _RRange<TRange>;
        requires RFwdIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `FwdRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RFwdIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutFwdRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutFwdRange = requires
    {
        requires _RMutRange<TRange>;
        requires RFwdIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutFwdIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutFwdRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutFwdRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RFwdIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutFwdIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `BidiRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RBidiRange = requires
    {
        requires _RRange<TRange>;
        requires RBidiIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `BidiRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBidiRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RBidiIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutBidiRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutBidiRange = requires
    {
        requires _RMutRange<TRange>;
        requires RBidiIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutBidiIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutBidiRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutBidiRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RBidiIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutBidiIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `JumpRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RJumpRange = requires
    {
        requires _RRange<TRange>;
        requires RJumpIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `JumpRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RJumpRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RJumpIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutJumpRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutJumpRange = requires
    {
        requires _RMutRange<TRange>;
        requires RJumpIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutJumpIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutJumpRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutJumpRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RJumpIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutJumpIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `ArrayRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RArrayRange = requires
    {
        requires _RRange<TRange>;
        requires RArrayIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `ArrayRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrayRangeOf = requires
    {
        requires _RRangeOf<TRange, T>;
        requires RArrayIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutArrayRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutArrayRange = requires
    {
        requires _RMutRange<TRange>;
        requires RArrayIterPair<typename TRange::TIter, typename TRange::TIterEnd>;
        requires RMutArrayIterPair<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutArrayRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutArrayRangeOf = requires
    {
        requires _RMutRangeOf<TRange, T>;
        requires RArrayIterPairOf<typename TRange::TIter, typename TRange::TIterEnd, T>;
        requires RMutArrayIterPairOf<typename TRange::TMutIter, typename TRange::TMutIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `RevRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename>
    concept RRevRange = requires
    {
        requires RBidiRange<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `RevRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RRevRangeOf = requires
    {
        requires RBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutRevRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutRevRange = requires
    {
        requires RMutBidiRange<TRange>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutRevRange` of type `T`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMutRevRangeOf = requires
    {
        requires RMutBidiRangeOf<TRange, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `CommonRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RCommonRange = requires
    {
        requires _RRange<TRange>;
        requires RSameAs<typename TRange::TIter, typename TRange::TIterEnd>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TRange` is `MutCommonRange`.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutCommonRange = requires
    {
        requires RCommonRange<TRange>;
        requires RSameAs<typename TRange::TMutIter, typename TRange::TMutIterEnd>;
    };
}
