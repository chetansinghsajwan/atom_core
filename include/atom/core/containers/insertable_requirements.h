#pragma once
#include "atom/core/core.h"
#include "atom/core/range.h"

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// `insertable` represents a type that allows inserting objects of some type into it.
    ///
    /// `front_insertable` represents a type that allows inserting objects of some type
    /// into front of container.
    ///
    /// `front_insertable` represents a type that allows inserting objects of some type
    /// into back of container.
    ///
    /// `key_insertable` represents a type that allows inserting objects of some type
    /// at specified index of container.
    ///
    /// `index_insertable` represents a type that allows inserting objects of some type
    /// at specified index of container.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_t` is `insertable` for type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_t, typename value_t>
    concept rinsertable =
        requires(insertable_t insertable, value_t el, mock::range<value_t> range)
    {
        insertable.insert(el);
        insertable.insert(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_t` is `front_insertable` for type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename front_insertable_t, typename value_t>
    concept rfront_insertable = 
        requires(front_insertable_t insertable, value_t el, mock::range<value_t> range)
    {
        insertable.insert_front(el);
        insertable.insert_front(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_t` is `back_insertable` for type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_t, typename value_t>
    concept rback_insertable =
        requires(insertable_t insertable, value_t el, mock::range<value_t> range)
    {
        insertable.insert_back(el);
        insertable.insert_back(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_t` is `key_insertable` for type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_t, typename key_t, typename value_t>
    concept rkey_insertable = requires(insertable_t insertable, key_t key, value_t el,
        mock::range<value_t> range)
    {
        insertable.insert(key, el);
        insertable.insert(key, range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_t` is `index_insertable` for type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_t, typename value_t>
    concept rindex_insertable = rkey_insertable<insertable_t, usize, value_t>;
}

// clang-format on
