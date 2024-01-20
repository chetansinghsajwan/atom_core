#pragma once
#include "atom/range.h"

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
    /// ensures `insertable_type` is `insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_type, typename type>
    concept rinsertable =
        requires(insertable_type insertable, type el, range_req_mock<type> range) {
            insertable.insert(el);
            insertable.insert(range);
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_type` is `front_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tfront_insertable, typename type>
    concept rfront_insertable =
        requires(tfront_insertable insertable, type el, range_req_mock<type> range) {
            insertable.insert_front(el);
            insertable.insert_front(range);
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_type` is `back_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_type, typename type>
    concept rback_insertable =
        requires(insertable_type insertable, type el, range_req_mock<type> range) {
            insertable.insert_back(el);
            insertable.insert_back(range);
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_type` is `key_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_type, typename tkey, typename type>
    concept rkey_insertable =
        requires(insertable_type insertable, tkey key, type el, range_req_mock<type> range) {
            insertable.insert(key, el);
            insertable.insert(key, range);
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures `insertable_type` is `index_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename insertable_type, typename type>
    concept rindex_insertable = rkey_insertable<insertable_type, usize, type>;
}
