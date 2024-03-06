#pragma once
#include "atom/core/core.h"
#include "atom/core/tti.h"
#include "atom/core/range.h"

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `rinsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class insertable
    {
        ATOM_STATIC_ASSERTS(tti::is_pure<value_type>);

    public:
        auto insert(const value_type& el);
        auto insert(value_type&& el);

        template <typename range_type>
        auto insert(const range_type& range)
            requires(rrange_of<range_type, value_type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rfront_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class front_insertable
    {
        ATOM_STATIC_ASSERTS(tti::is_pure<value_type>);

    public:
        auto insert_front(const value_type& el);
        auto insert_front(value_type&& el);

        template <typename range_type>
        auto insert_front(const range_type& range)
            requires(rrange_of<range_type, value_type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rback_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class back_insertable
    {
        ATOM_STATIC_ASSERTS(tti::is_pure<value_type>);

    public:
        auto insert_back(const value_type& el);
        auto insert_back(value_type&& el);

        template <typename range_type>
        auto insert_back(const range_type& range)
            requires(rrange_of<range_type, value_type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rkey_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename key_type, typename value_type>
    class key_insertable
    {
        ATOM_STATIC_ASSERTS(tti::is_pure<value_type>);

    public:
        auto insert(key_type key, const value_type& el);
        auto insert(key_type key, value_type&& el);

        template <typename range_type>
        auto insert(key_type key, const range_type& range)
            requires(rrange_of<range_type, value_type>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rindex_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    using index_insertable = key_insertable<usize, value_type>;
}
