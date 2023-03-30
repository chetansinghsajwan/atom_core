#pragma once
#include "Atom/Containers/Iterable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Insertable: Represents a type that allows inserting elements of type {Element} into it.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {Insertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TElement>
    concept RInsertable = requires(TInsertable insertable, TElement element)
    {
        insertable.Insert(element);
        insertable.Insert(ConstIterableTestImpl<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// FrontInsertable: Represents a type that allows inserting elements of type {Element} 
    /// into front of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {FrontInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrontInsertable, typename TElement>
    concept RFrontInsertable = requires(TFrontInsertable insertable, TElement element)
    {
        insertable.InsertFront(element);
        insertable.InsertFront(ConstIterableTestImpl<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// FrontInsertable: Represents a type that allows inserting elements of type {Element} 
    /// into back of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {BackInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TBackInsertable, typename TElement>
    concept RBackInsertable = requires(TBackInsertable insertable, TElement element)
    {
        insertable.InsertBack(element);
        insertable.InsertBack(ConstIterableTestImpl<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// IndexInsertable: Represents a type that allows inserting elements of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {IndexInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIndexInsertable, typename TElement>
    concept RIndexInsertable = requires(TIndexInsertable insertable, TElement element)
    {
        insertable.InsertAt(0, element);
        insertable.InsertAt(0, ConstIterableTestImpl<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// Type to test if RBackInsertable is accepted during defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BackInsertableTestImpl
    {
        constexpr void InsertBack(const T&) noexcept { }
        constexpr void InsertBack(const RConstIterable<T> auto& iterable) noexcept { }
    };

    static_assert(RBackInsertable<BackInsertableTestImpl<int>, int>,
        "BackInsertableTestImpl does not satisfy RBackInsertable requirements.");
}