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
    public:
        template <typename U>
        fn Insert(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        fn Insert(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RFrontInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FrontInsertableReqMock
    {
    public:
        template <typename U>
        fn InsertFront(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        fn InsertFront(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RBackInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BackInsertableReqMock
    {
    public:
        template <typename U>
        fn InsertBack(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        fn InsertBack(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RKeyInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename TKey, typename T>
    class KeyInsertableReqMock
    {
    public:
        template <typename U>
        fn Insert(TKey key, U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        fn Insert(TKey key, const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// {RIndexInsertable} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;
}
