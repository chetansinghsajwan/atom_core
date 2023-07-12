#pragma once
#include "InsertableReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {RInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct InsertableReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        void Insert(U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        void Insert(const TRange& range);
    };

    static_assert(RInsertable<InsertableReqMock<int>, int>,
        "{InsertableReqMock} does not satisfy {RInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RFrontInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct FrontInsertableReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        void InsertFront(U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        void InsertFront(const TRange& range);
    };

    static_assert(RFrontInsertable<FrontInsertableReqMock<int>, int>,
        "{FrontInsertableReqMock} does not satisfy {RFrontInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RBackInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct BackInsertableReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        void InsertBack(U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        void InsertBack(const TRange& range);
    };

    static_assert(RBackInsertable<BackInsertableReqMock<int>, int>,
        "{BackInsertableReqMock} does not satisfy {RBackInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RKeyInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname TKey, tname T>
    struct KeyInsertableReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        void Insert(TKey key, U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        void Insert(TKey key, const TRange& range);
    };

    static_assert(RKeyInsertable<KeyInsertableReqMock<char, int>, char, int>,
        "{KeyInsertableReqMock} does not satisfy {RKeyInsertable} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RIndexInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;

    static_assert(RIndexInsertable<IndexInsertableReqMock<int>, int>,
        "{IndexInsertableReqMock} does not satisfy {RIndexInsertable} requirements.");
}
