#pragma once
#include "insertable_req.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// `rinsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class insertable_req_mock
    {
    public:
        template <typename u>
        auto insert(u&& el)
            requires(rsame_as_unqualified<u, type>);

        template <typename range_type>
        auto insert(const range_type& range)
            requires(rrange_of<range_type, type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rfront_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class front_insertable_req_mock
    {
    public:
        template <typename u>
        auto insert_front(u&& el)
            requires(rsame_as_unqualified<u, type>);

        template <typename range_type>
        auto insert_front(const range_type& range)
            requires(rrange_of<range_type, type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rback_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class back_insertable_req_mock
    {
    public:
        template <typename u>
        auto insert_back(u&& el)
            requires(rsame_as_unqualified<u, type>);

        template <typename range_type>
        auto insert_back(const range_type& range)
            requires(rrange_of<range_type, type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rkey_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename tkey, typename type>
    class key_insertable_req_mock
    {
    public:
        template <typename u>
        auto insert(tkey key, u&& el)
            requires(rsame_as_unqualified<u, type>);

        template <typename range_type>
        auto insert(tkey key, const range_type& range)
            requires(rrange_of<range_type, type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rindex_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    using index_insertable_req_mock = key_insertable_req_mock<usize, type>;
}
