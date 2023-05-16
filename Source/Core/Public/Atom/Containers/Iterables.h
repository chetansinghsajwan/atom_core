#pragma once
#include "Atom/Containers/Iterators.h"

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

namespace Atom::Internal
{
    /// Type to test if type implementing RFwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeMock
    {
        using TIterator = FwdItMock<T>;

        TIterator Iterator();
    };

    static_assert(RFwdRange<FwdRangeMock<int>, int>,
        "FwdRangeMock does not meet RFwdRange requirements.");

    /// Type to test if type implementing RBwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdRangeMock
    {
        using TIterator = BwdItMock<T>;

        TIterator Iterator();
    };

    static_assert(RBwdRange<BwdRangeMock<int>, int>,
        "BwdRangeMock does not meet RBwdRange requirements.");

    /// Type to test if type implementing RFwdJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpRangeMock
    {
        using TIterator = FwdJumpItMock<T>;

        TIterator Iterator();
    };

    static_assert(RFwdJumpRange<FwdJumpRangeMock<int>, int>,
        "FwdJumpRangeMock does not meet RFwdJumpRange requirements.");

    /// Type to test if type implementing RBwdJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpRangeMock
    {
        using TIterator = BwdJumpItMock<T>;

        TIterator Iterator();
    };

    static_assert(RBwdJumpRange<BwdJumpRangeMock<int>, int>,
        "BwdJumpRangeMock does not meet RBwdJumpRange requirements.");

    /// Type to test if type implementing RTwoWayRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayRangeMock
    {
        using TIterator = TwoWayItMock<T>;

        TIterator Iterator();
    };

    static_assert(RTwoWayRange<TwoWayRangeMock<int>, int>,
        "TwoWayRangeMock does not meet RTwoWayRange requirements.");

    /// Type to test if type implementing RTwoWayJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpRangeMock
    {
        using TIterator = TwoWayJumpItMock<T>;

        TIterator Iterator();
    };

    static_assert(RTwoWayJumpRange<TwoWayJumpRangeMock<int>, int>,
        "TwoWayJumpRangeMock does not meet RTwoWayJumpRange requirements.");

    /// Type to test if type implementing RArrayRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayRangeMock
    {
        using TIterator = ArrayItMock<T>;

        TIterator Iterator();
    };

    static_assert(RArrayRange<ArrayRangeMock<int>, int>,
        "ArrayRangeMock does not meet RArrayRange requirements.");

//     /// Type to test if type implementing RMultiPassRange is accepted when defining concepts.
//     /// --------------------------------------------------------------------------------------------
//     template <typename T>
//     struct MultiPassRangeMock
//     {
//         using TIterator = MultiPassItMock<T>;
// 
//         TIterator Iterator();
//     };
// 
//     static_assert(RMultiPassRange<MultiPassRangeMock<int>, int>,
//         "MultiPassRangeMock does not meet RMultiPassRange requirements.");
}
