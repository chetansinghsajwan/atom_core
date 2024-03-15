#pragma once
#include "atom/core/core.h"
#include "atom/core/typeinfo.h"
#include "atom/core/range.h"

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `rinsertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class insertable
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<value_t>);

    public:
        auto insert(const value_t& el);
        auto insert(value_t&& el);

        template <typename range_t>
        auto insert(const range_t& range)
            requires(rrange_of<range_t, value_t>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rfront_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class front_insertable
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<value_t>);

    public:
        auto insert_front(const value_t& el);
        auto insert_front(value_t&& el);

        template <typename range_t>
        auto insert_front(const range_t& range)
            requires(rrange_of<range_t, value_t>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rback_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class back_insertable
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<value_t>);

    public:
        auto insert_back(const value_t& el);
        auto insert_back(value_t&& el);

        template <typename range_t>
        auto insert_back(const range_t& range)
            requires(rrange_of<range_t, value_t>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rkey_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename key_t, typename value_t>
    class key_insertable
    {
        ATOM_STATIC_ASSERTS(typeinfo::is_pure<value_t>);

    public:
        auto insert(key_t key, const value_t& el);
        auto insert(key_t key, value_t&& el);

        template <typename range_t>
        auto insert(key_t key, const range_t& range)
            requires(rrange_of<range_t, value_t>);
    };

    /// --------------------------------------------------------------------------------------------
    /// `rindex_insertable` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    using index_insertable = key_insertable<usize, value_t>;
}
