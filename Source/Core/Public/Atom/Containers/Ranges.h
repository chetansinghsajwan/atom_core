#pragma once
#include "Iterators.h"

namespace Atom::Private
{
    template <typename TRange, typename T>
    concept RRangeBase = requires(TRange range)
    {
        requires true;

        typename TRange::TIter;
        typename TRange::TEnd;

        { range.Begin() } -> RSameAs<typename TRange::TIter>;
        { range.End() } -> RSameAs<typename TRange::TEnd>;

        { range.begin() } -> RSameAs<typename TRange::TIter>;
        { range.end() } -> RSameAs<typename TRange::TEnd>;

        // requires !RConst<T> || requires
        // {
        //     { range.ConstBegin() } -> RSameAs<typename TRange::TIter>;
        //     { range.ConstEnd() } -> RSameAs<typename TRange::TEnd>;
        // };
    };
}

namespace Atom
{
    /// Ensures {TRange} is {FwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdRange = requires(TRange range)
    {
        requires Private::RRangeBase<TRange, T>;
        requires RFwdIter<typename TRange::TIter, typename TRange::TEnd, T>;
        // requires RFwdIter<decltype(range.Begin()), decltype(range.End()), T>;
    };

    /// Ensures {TRange} is {BwdRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {FwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RFwdJumpRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RFwdJumpIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {BwdJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RBwdJumpRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RBwdJumpIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {TwoWayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {TwoWayJumpRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RTwoWayJumpRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RTwoWayJumpIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {MultiPassRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RMultiPassRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RMultiPassIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {ArrayRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RArrayRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RArrayIter<typename TRange::TIter, typename TRange::TEnd, T>;
    };

    /// Ensures {TRange} is {CommonRange}.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename T>
    concept RCommonRange = requires
    {
        requires Private::RRangeBase<TRange, T>;
        requires RSameAs<typename TRange::TIter, typename TRange::TIter>;
    };
}
