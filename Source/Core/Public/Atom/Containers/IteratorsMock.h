#pragma once
#include "Iterators.h"

namespace Atom::Internal
{
    /// Type to test if a type implementing {RFwdIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdIterMock
    {
        T& operator * ();
        bool operator == (NullPtr end);
        FwdIterMock& operator ++ (int);
    };

    static_assert(RFwdIter<FwdIterMock<int>, NullPtr, int>,
        "FwdIterMock does not meet RFwdIter requirements.");

    /// Type to test if a type implementing {RBwdIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdIterMock
    {
        T& operator * ();
        bool operator == (NullPtr end);
        BwdIterMock& operator -- (int);
    };

    static_assert(RBwdIter<BwdIterMock<int>, NullPtr, int>,
        "BwdIterMock does not meet RBwdIter requirements.");

    /// Type to test if a type implementing {RFwdJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpIterMock: FwdIterMock<T>
    {
        FwdJumpIterMock operator + (usize steps);
        usize operator - (const FwdJumpIterMock& that);
    };

    static_assert(RFwdJumpIter<FwdJumpIterMock<int>, NullPtr, int>,
        "FwdJumpIterMock does not meet RFwdJumpIter requirements.");

    /// Type to test if a type implementing {RBwdJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpIterMock: BwdIterMock<T>
    {
        BwdJumpIterMock operator - (usize steps);
        usize operator - (const BwdJumpIterMock& that);
    };

    static_assert(RBwdJumpIter<BwdJumpIterMock<int>, NullPtr, int>,
        "BwdJumpIterMock does not meet RBwdJumpIter requirements.");

    /// Type to test if a type implementing {RTwoWayIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayIterMock: FwdIterMock<T>, BwdIterMock<T>
    {
        T& operator * ();
        bool operator == (NullPtr end);
    };

    static_assert(RTwoWayIter<TwoWayIterMock<int>, NullPtr, int>,
        "TwoWayIterMock does not meet RTwoWayIter requirements.");

    /// Type to test if a type implementing {RTwoWayJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpIterMock: FwdJumpIterMock<T>, BwdJumpIterMock<T>
    {
        T& operator * ();
        bool operator == (NullPtr end);
        TwoWayJumpIterMock operator + (usize steps);
        TwoWayJumpIterMock operator - (usize steps);
        usize operator - (const TwoWayJumpIterMock& that);
    };

    static_assert(RTwoWayJumpIter<TwoWayJumpIterMock<int>, NullPtr, int>,
        "TwoWayJumpIterMock does not meet RTwoWayJumpIter requirements.");

    /// Type to test if a type implementing {RMultiPassIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MultiPassIterMock: FwdIterMock<T>,
        public MultiPassIterTag { };

    static_assert(RMultiPassIter<MultiPassIterMock<int>, NullPtr, int>,
        "MultiPassIterMock does not meet RMultiPassIter requirements.");

    /// Type to test if a type implementing {RArrayIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayIterMock: TwoWayJumpIterMock<T>,
        public ArrayIterTag { };

    static_assert(RArrayIter<ArrayIterMock<int>, NullPtr, int>,
        "ArrayIterMock does not meet RArrayIter requirements.");
}
