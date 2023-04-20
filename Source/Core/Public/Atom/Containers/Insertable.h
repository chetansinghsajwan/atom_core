#pragma once
#include "Atom/Containers/Iterable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {Insertable} represents a type that allows inserting elements of type {Element} into it.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {Insertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TElement>
    concept RInsertable = requires(TInsertable insertable, TElement element)
    {
        insertable.Insert(element);
        insertable.Insert(Internal::InputIteratorMock<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// {FrontInsertable} represents a type that allows inserting elements of type {Element} 
    /// into front of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {FrontInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrontInsertable, typename TElement>
    concept RFrontInsertable = requires(TFrontInsertable insertable, TElement element)
    {
        insertable.InsertFront(element);
        insertable.InsertFront(Internal::InputIteratorMock<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// {FrontInsertable} represents a type that allows inserting elements of type {Element} 
    /// into back of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {BackInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TElement>
    concept RBackInsertable = requires(TInsertable insertable, TElement element)
    {
        insertable.InsertBack(element);
        insertable.InsertBack(Internal::InputIteratorMock<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// {KeyInsertable} represents a type that allows inserting elements of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {KeyInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TKey, typename TElement>
    concept RKeyInsertable = requires(TInsertable insertable, TKey key, TElement element)
    {
        insertable.Insert(key, element);
        insertable.Insert(key, Internal::InputIteratorMock<TElement>());
    };

    /// --------------------------------------------------------------------------------------------
    /// {IndexInsertable} represents a type that allows inserting elements of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TInsertable} is {IndexInsertable} for type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TElement>
    concept RIndexInsertable = RKeyInsertable<TInsertable, usize, TElement>;
}

namespace Atom::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// Type to test if a type implementing {RBackInsertable} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BackInsertableMock
    {
        void InsertBack(const T&);
        void InsertBack(T&&);

        template <RInputIterator<T> TInput>
        void InsertBack(TInput in);
    };

    static_assert(RBackInsertable<BackInsertableMock<int>, int>,
        "BackInsertableMock does not satisfy RBackInsertable requirements.");
}