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
    /// ensures `tinsertable` is `insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tinsertable, typename type>
    concept rinsertable = requires(tinsertable insertable, type el, range_req_mock<type> range) {
        insertable.insert(el);
        insertable.insert(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `tinsertable` is `front_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tfront_insertable, typename type>
    concept rfront_insertable = requires(tfront_insertable insertable, type el, range_req_mock<type> range) {
        insertable.insert_front(el);
        insertable.insert_front(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `tinsertable` is `back_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tinsertable, typename type>
    concept rback_insertable = requires(tinsertable insertable, type el, range_req_mock<type> range) {
        insertable.insert_back(el);
        insertable.insert_back(range);
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `tinsertable` is `key_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tinsertable, typename tkey, typename type>
    concept rkey_insertable =
        requires(tinsertable insertable, tkey key, type el, range_req_mock<type> range) {
            insertable.insert(key, el);
            insertable.insert(key, range);
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures `tinsertable` is `index_insertable` for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tinsertable, typename type>
    concept rindex_insertable = rkey_insertable<tinsertable, usize, type>;
}
