#pragma once
#include "Ranges.h"
#include "IteratorsMock.h"

namespace Atom::Private
{
    /// Type to test if type implementing RFwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename TInIter, typename TIterEnd>
    struct RangeBaseMock
    {
        using TIter = TInIter;
        using TEnd = TIterEnd;

        TIter Begin();
        TIterEnd End();

        TIter begin();
        TIterEnd end();
    };
}

namespace Atom::Internal
{
    /// Type to test if type implementing RFwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeMock: Private::RangeBaseMock<FwdIterMock<T>, NullPtr>
    {
        using TImpl = Private::RangeBaseMock<FwdIterMock<T>, NullPtr>;
        
        using TIter = typename TImpl::TIter;
        using TEnd = typename TImpl::TEnd;
    };

    static_assert(RFwdRange<FwdRangeMock<int>, int>,
        "FwdRangeMock<int> does not meet RFwdRange<int> requirements.");

    static_assert(RFwdRange<FwdRangeMock<const int>, const int>,
        "FwdRangeMock<const int> does not meet RFwdRange<const int> requirements.");

    /// Type to test if type implementing RBwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdRangeMock: Private::RangeBaseMock<BwdIterMock<T>, NullPtr>
    {
        using TImpl = Private::RangeBaseMock<BwdIterMock<T>, NullPtr>;
        
        using TIter = typename TImpl::TIter;
        using TEnd = typename TImpl::TEnd;
    };

    static_assert(RBwdRange<BwdRangeMock<int>, int>,
        "BwdRangeMock<int> does not meet RBwdRange<int> requirements.");

    static_assert(RBwdRange<BwdRangeMock<const int>, const int>,
        "BwdRangeMock<const int> does not meet RBwdRange<const int> requirements.");

    /// Type to test if type implementing RFwdRange is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpRangeMock: Private::RangeBaseMock<FwdJumpIterMock<T>, NullPtr>
    {
        using TImpl = Private::RangeBaseMock<FwdJumpIterMock<T>, NullPtr>;
        
        using TIter = typename TImpl::TIter;
        using TEnd = typename TImpl::TEnd;
    };

    static_assert(RFwdJumpRange<FwdJumpRangeMock<int>, int>,
        "FwdJumpRangeMock<int> does not meet RFwdJumpRange<int> requirements.");

    static_assert(RFwdJumpRange<FwdJumpRangeMock<const int>, const int>,
        "FwdJumpRangeMock<const int> does not meet RFwdJumpRange<const int> requirements.");

    // TODO: Implementing remaining mock objects.
}
