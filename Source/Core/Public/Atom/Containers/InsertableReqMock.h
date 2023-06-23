#pragma once
#include "InsertableReq.h"
#include "RangeReqMock.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {RInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct InsertableReqMock
    {
        template <typename U>
        requires RSameAsUnqualified<U, T>
        void Insert(U&& el);

        template <typename TRange>
        requires RRangeOf<TRange, T>
        void Insert(const TRange& range);
    };

    static_assert(RInsertable<InsertableReqMock<int>, int>,
        "{InsertableReqMock} does not satisfy {RInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RFrontInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FrontInsertableReqMock
    {
        template <typename U>
        requires RSameAsUnqualified<U, T>
        void InsertFront(U&& el);

        template <typename TRange>
        requires RRangeOf<TRange, T>
        void InsertFront(const TRange& range);
    };

    static_assert(RFrontInsertable<FrontInsertableReqMock<int>, int>,
        "{FrontInsertableReqMock} does not satisfy {RFrontInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RBackInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BackInsertableReqMock
    {
        template <typename U>
        requires RSameAsUnqualified<U, T>
        void InsertBack(U&& el);

        template <typename TRange>
        requires RRangeOf<TRange, T>
        void InsertBack(const TRange& range);
    };

    static_assert(RBackInsertable<BackInsertableReqMock<int>, int>,
        "{BackInsertableReqMock} does not satisfy {RBackInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RKeyInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename TKey, typename T>
    struct KeyInsertableReqMock
    {
        template <typename U>
        requires RSameAsUnqualified<U, T>
        void Insert(TKey key, U&& el);

        template <typename TRange>
        requires RRangeOf<TRange, T>
        void Insert(TKey key, const TRange& range);
    };

    static_assert(RKeyInsertable<KeyInsertableReqMock<char, int>, char, int>,
        "{KeyInsertableReqMock} does not satisfy {RKeyInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RIndexInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;

    static_assert(RIndexInsertable<IndexInsertableReqMock<int>, int>,
        "{IndexInsertableReqMock} does not satisfy {RIndexInsertable} requirements.");
}
