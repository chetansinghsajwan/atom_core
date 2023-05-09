#pragma once
#include "Atom/Containers/Iterator.h"

namespace Atom
{
    /// {OneWayIterable} is a type which provides {OneWayIterator}.
    /// {TwoWayIterable} is a type which provides {TwoWayIterator}.
    /// {DirectIterable} is a type which provides {DirectIterator}.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TIterable} is {OneWayIterable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterable, typename T>
    concept ROneWayIterable = requires(TIterable iterable)
    {
        { iterable.Iterator() } -> ROneWayIterator<T>;
    };

    template <typename TIterable, typename T>
    concept RInputIterable = ROneWayIterable<TIterable, T>;

    /// Ensures {TIterable} is {TwoWayIterable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterable, typename T>
    concept RTwoWayIterable = requires(TIterable iterable)
    {
        { iterable.Iterator() } -> RTwoWayIterator<T>;
    };

    /// Ensures {TIterable} is {DirectIterable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterable, typename T>
    concept RDirectIterable = requires(TIterable iterable)
    {
        { iterable.Iterator() } -> RDirectIterator<T>;
    };
}

namespace Atom::Internal
{
    /// Type to test if a type implementing {ROneWayIterable} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct OneWayIterableMock
    {
        OneWayIteratorMock<T> Iterator();
    };

    static_assert(ROneWayIterable<OneWayIterableMock<int>, int>,
        "OneWayIterableMock does not meet ROneWayIterable requirements.");

    /// Type to test if a type implementing {RTwoWayIterable} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayIterableMock: OneWayIterableMock<T>
    {
        TwoWayIteratorMock<T> Iterator();
    };

    static_assert(RTwoWayIterable<TwoWayIterableMock<int>, int>,
        "TwoWayIterableMock does not meet RTwoWayIterable requirements.");

    /// Type to test if a type implementing {RDirectIterable} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct DirectIterableMock: TwoWayIterableMock<T>
    {
        DirectIteratorMock<T> Iterator();
    };

    static_assert(RDirectIterable<DirectIterableMock<int>, int>,
        "DirectIterableMock does not meet RDirectIterable requirements.");
}