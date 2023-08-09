#pragma once
#include "InsertableReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {RInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class InsertableReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn Insert(U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn Insert(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RFrontInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FrontInsertableReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn InsertFront(U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn InsertFront(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RBackInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BackInsertableReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn InsertBack(U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn InsertBack(const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RKeyInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename TKey, typename T>
    class KeyInsertableReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn Insert(TKey key, U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn Insert(TKey key, const TRange& range);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RIndexInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;
}
