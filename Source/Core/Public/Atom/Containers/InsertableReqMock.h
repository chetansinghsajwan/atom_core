#pragma once
#include "InsertableReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {RInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class InsertableReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn Insert(U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn Insert(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RFrontInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class FrontInsertableReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn InsertFront(U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn InsertFront(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RBackInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class BackInsertableReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn InsertBack(U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn InsertBack(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RKeyInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname TKey, tname T>
    class KeyInsertableReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn Insert(TKey key, U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn Insert(TKey key, const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RIndexInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;
}
