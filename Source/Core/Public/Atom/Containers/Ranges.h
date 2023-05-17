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
        requires RFwdIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {BwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {FwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RFwdJumpIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {BwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdJumpIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {TwoWayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {TwoWayJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayJumpRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayJumpIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {ArrayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrayRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RArrayIter<typename TRange::TIterator, T>;
    };

    /// Ensures {TRange} is {MultiPassRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMultiPassRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RMultiPassIter<typename TRange::TIterator, T>;
    };
}
