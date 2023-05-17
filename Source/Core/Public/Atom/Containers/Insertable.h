#pragma once
#include "Ranges.h"
#include "RangesMock.h"

namespace Atom
{
    /// {Insertable} represents a type that allows inserting objs of type {Element} into it.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TInsertable} is {Insertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RInsertable = requires(TInsertable insertable, T obj)
    {
        insertable.Insert(obj);
        insertable.Insert(Internal::FwdIterMock<T>());
    };

    /// {FrontInsertable} represents a type that allows inserting objs of type {Element} 
    /// into front of container.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TInsertable} is {FrontInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrontInsertable, typename T>
    concept RFrontInsertable = requires(TFrontInsertable insertable, T obj)
    {
        insertable.InsertFront(obj);
        insertable.InsertFront(Internal::FwdIterMock<T>());
    };

    /// {FrontInsertable} represents a type that allows inserting objs of type {Element} 
    /// into back of container.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TInsertable} is {BackInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RBackInsertable = requires(TInsertable insertable, T obj)
    {
        insertable.InsertBack(obj);
        insertable.InsertBack(Internal::FwdIterMock<T>());
    };

    /// {KeyInsertable} represents a type that allows inserting objs of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TInsertable} is {KeyInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename TKey, typename T>
    concept RKeyInsertable = requires(TInsertable insertable, TKey key, T obj)
    {
        insertable.Insert(key, obj);
        insertable.Insert(key, Internal::FwdIterMock<T>());
    };

    /// {IndexInsertable} represents a type that allows inserting objs of type {Element} 
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// Ensures {TInsertable} is {IndexInsertable} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TInsertable, typename T>
    concept RIndexInsertable = RKeyInsertable<TInsertable, usize, T>;
}

namespace Atom::Internal
{
    /// Type to test if a type implementing {RBackInsertable} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BackInsertableMock
    {
        void InsertBack(const T&);
        void InsertBack(T&&);

        template <RFwdIter<T> TInput>
        void InsertBack(TInput in);
    };

    static_assert(RBackInsertable<BackInsertableMock<int>, int>,
        "BackInsertableMock does not satisfy RBackInsertable requirements.");
}