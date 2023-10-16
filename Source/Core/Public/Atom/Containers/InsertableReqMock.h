#pragma once
#include "InsertableReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// `RInsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class InsertableReqMock
    {
    public:
        template <typename U>
        auto Insert(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        auto Insert(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `RFrontInsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FrontInsertableReqMock
    {
    public:
        template <typename U>
        auto InsertFront(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        auto InsertFront(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `RBackInsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BackInsertableReqMock
    {
    public:
        template <typename U>
        auto InsertBack(U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        auto InsertBack(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `RKeyInsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename TKey, typename T>
    class KeyInsertableReqMock
    {
    public:
        template <typename U>
        auto Insert(TKey key, U&& el)
            requires(RSameAsUnqualified<U, T>);

        template <typename TRange>
        auto Insert(TKey key, const TRange& range)
            requires(RRangeOf<TRange, T>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `RIndexInsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    using IndexInsertableReqMock = KeyInsertableReqMock<usize, T>;
}
