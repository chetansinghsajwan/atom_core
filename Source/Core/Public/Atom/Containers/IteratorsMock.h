#pragma once
#include "Iterators.h"

namespace Atom::Internal
{
    /// Type to test if a type implementing {RFwdIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdIterMock
    {
        T& Get();

        bool Next();
        bool HasNext() const;
    };

    static_assert(RFwdIter<FwdIterMock<int>, int>,
        "FwdIterMock does not meet RFwdIter requirements.");

    /// Type to test if a type implementing {RBwdIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdIterMock
    {
        T& Get();

        bool Prev();
        bool HasPrev() const;
    };

    static_assert(RBwdIter<BwdIterMock<int>, int>,
        "BwdIterMock does not meet RBwdIter requirements.");

    /// Type to test if a type implementing {RFwdJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpIterMock: FwdIterMock<T>
    {
        using FwdIterMock<T>::Next;

        bool Next(usize steps);
        usize NextRange() const;
    };

    static_assert(RFwdJumpIter<FwdJumpIterMock<int>, int>,
        "FwdJumpIterMock does not meet RFwdJumpIter requirements.");

    /// Type to test if a type implementing {RBwdJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpIterMock: BwdIterMock<T>
    {
        using BwdIterMock<T>::Prev;

        bool Prev(usize steps);
        usize PrevRange() const;
    };

    static_assert(RBwdJumpIter<BwdJumpIterMock<int>, int>,
        "BwdJumpIterMock does not meet RBwdJumpIter requirements.");

    /// Type to test if a type implementing {RTwoWayIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayIterMock: FwdIterMock<T>, BwdIterMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayIter<TwoWayIterMock<int>, int>,
        "TwoWayIterMock does not meet RTwoWayIter requirements.");

    /// Type to test if a type implementing {RTwoWayJumpIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpIterMock: FwdJumpIterMock<T>, BwdJumpIterMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayJumpIter<TwoWayJumpIterMock<int>, int>,
        "TwoWayJumpIterMock does not meet RTwoWayJumpIter requirements.");

    /// Type to test if a type implementing {RArrayIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayIterMock: TwoWayJumpIterMock<T>
    {
        T* Data();
    };

    static_assert(RArrayIter<ArrayIterMock<int>, int>,
        "ArrayIterMock does not meet RArrayIter requirements.");

    /// Type to test if a type implementing {RMultiPassIter} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MultiPassIterMock: FwdIterMock<T> { };

    static_assert(RMultiPassIter<MultiPassIterMock<int>, int>,
        "MultiPassIterMock does not meet RMultiPassIter requirements.");
}
