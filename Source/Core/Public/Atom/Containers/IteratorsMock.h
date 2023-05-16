#pragma once
#include "Iterators.h"

namespace Atom::Internal
{
    /// Type to test if a type implementing {RFwdIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdItMock
    {
        T& Get();

        bool Next();
        bool HasNext() const;
    };

    static_assert(RFwdIt<FwdItMock<int>, int>,
        "FwdItMock does not meet RFwdIt requirements.");

    /// Type to test if a type implementing {RBwdIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdItMock
    {
        T& Get();

        bool Prev();
        bool HasPrev() const;
    };

    static_assert(RBwdIt<BwdItMock<int>, int>,
        "BwdItMock does not meet RBwdIt requirements.");

    /// Type to test if a type implementing {RFwdJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpItMock: FwdItMock<T>
    {
        using FwdItMock<T>::Next;

        bool Next(usize steps);
        usize NextRange() const;
    };

    static_assert(RFwdJumpIt<FwdJumpItMock<int>, int>,
        "FwdJumpItMock does not meet RFwdJumpIt requirements.");

    /// Type to test if a type implementing {RBwdJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpItMock: BwdItMock<T>
    {
        using BwdItMock<T>::Prev;

        bool Prev(usize steps);
        usize PrevRange() const;
    };

    static_assert(RBwdJumpIt<BwdJumpItMock<int>, int>,
        "BwdJumpItMock does not meet RBwdJumpIt requirements.");

    /// Type to test if a type implementing {RTwoWayIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayItMock: FwdItMock<T>, BwdItMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayIt<TwoWayItMock<int>, int>,
        "TwoWayItMock does not meet RTwoWayIt requirements.");

    /// Type to test if a type implementing {RTwoWayJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpItMock: FwdJumpItMock<T>, BwdJumpItMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayJumpIt<TwoWayJumpItMock<int>, int>,
        "TwoWayJumpItMock does not meet RTwoWayJumpIt requirements.");

    /// Type to test if a type implementing {RArrayIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayItMock: TwoWayJumpItMock<T>
    {
        T* Data();
    };

    static_assert(RArrayIt<ArrayItMock<int>, int>,
        "ArrayItMock does not meet RArrayIt requirements.");

    /// Type to test if a type implementing {RMultiPassIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MultiPassItMock: FwdItMock<T> { };

    static_assert(RMultiPassIt<MultiPassItMock<int>, int>,
        "MultiPassItMock does not meet RMultiPassIt requirements.");
}
