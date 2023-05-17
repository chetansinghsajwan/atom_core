#pragma once
#include "Ranges.h"
#include "IteratorsMock.h"

namespace Atom::Internal
{
    /// Type to test if type implementing RFwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeMock
    {
        using TIterator = FwdIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RFwdRange<FwdRangeMock<int>, int>,
        "FwdRangeMock does not meet RFwdRange requirements.");

    /// Type to test if type implementing RBwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdRangeMock
    {
        using TIterator = BwdIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RBwdRange<BwdRangeMock<int>, int>,
        "BwdRangeMock does not meet RBwdRange requirements.");

    /// Type to test if type implementing RFwdJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpRangeMock
    {
        using TIterator = FwdJumpIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RFwdJumpRange<FwdJumpRangeMock<int>, int>,
        "FwdJumpRangeMock does not meet RFwdJumpRange requirements.");

    /// Type to test if type implementing RBwdJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpRangeMock
    {
        using TIterator = BwdJumpIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RBwdJumpRange<BwdJumpRangeMock<int>, int>,
        "BwdJumpRangeMock does not meet RBwdJumpRange requirements.");

    /// Type to test if type implementing RTwoWayRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayRangeMock
    {
        using TIterator = TwoWayIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RTwoWayRange<TwoWayRangeMock<int>, int>,
        "TwoWayRangeMock does not meet RTwoWayRange requirements.");

    /// Type to test if type implementing RTwoWayJumpRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpRangeMock
    {
        using TIterator = TwoWayJumpIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RTwoWayJumpRange<TwoWayJumpRangeMock<int>, int>,
        "TwoWayJumpRangeMock does not meet RTwoWayJumpRange requirements.");

    /// Type to test if type implementing RArrayRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayRangeMock
    {
        using TIterator = ArrayIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RArrayRange<ArrayRangeMock<int>, int>,
        "ArrayRangeMock does not meet RArrayRange requirements.");

    /// Type to test if type implementing RMultiPassRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MultiPassRangeMock
    {
        using TIterator = MultiPassIterMock<T>;

        TIterator Iterator();
    };

    static_assert(RMultiPassRange<MultiPassRangeMock<int>, int>,
        "MultiPassRangeMock does not meet RMultiPassRange requirements.");
}
