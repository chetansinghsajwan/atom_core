#pragma once
#include "Iterators.h"

namespace Atom::Private
{
    template <typename TRange, typename T>
    concept RRangeBase = true;
//     concept RRangeBase = requires(TRange range)
//     {
//         { typename TRange::TIterator };
// 
//         { range.Iterator() } -> RSameAs<typename TRange::TIterator>;
//     };
}

namespace Atom
{
    /// Ensures {TRange} is {FwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RFwdIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {BwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {FwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RFwdJumpIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {BwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdJumpIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {TwoWayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {TwoWayJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayJumpIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {ArrayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrayRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RArrayIt<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {MultiPassRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMultiPassRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RMultiPassIt<typename TRange::TIterator, T>;
    };
}
