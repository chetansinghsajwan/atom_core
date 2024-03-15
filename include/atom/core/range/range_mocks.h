#pragma once
#include "atom/core/range/iter_mocks.h"

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iter_t, typename in_iter_end_t, typename in_elem_t>
    class _range
    {
    public:
        using value_t = in_elem_t;
        using iter_t = in_iter_t;
        using iter_end_t = in_iter_end_t;

    public:
        auto get_iter() const -> iter_t;
        auto get_iter_end() const -> iter_end_t;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iter_t, typename in_iter_end_t, typename in_elem_t>
    class _mut_range
    {
    public:
        using value_t = in_elem_t;
        using iter_t = in_iter_t;
        using iter_end_t = in_iter_end_t;
        using mut_iter_t = in_iter_t;
        using mut_iter_end_t = in_iter_end_t;

    public:
        auto get_iter() const -> iter_t;
        auto get_iter_end() const -> iter_end_t;
        auto get_mut_iter() -> mut_iter_t;
        auto get_mut_iter_end() -> mut_iter_end_t;
    };
}

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `is_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class range: public _range<iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_range: public _mut_range<mut_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class fwd_range: public _range<fwd_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_fwd_range: public _mut_range<mut_fwd_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class bidi_range: public _range<bidi_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_bidi_range: public _mut_range<mut_bidi_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_jump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class jump_range: public _range<jump_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_jump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_jump_range: public _mut_range<mut_jump_iter<value_t>, iter_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class array_range: public _range<array_iter<value_t>, array_iter<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_array_range
        : public _mut_range<mut_array_iter<value_t>, mut_array_iter<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class common_range: public _range<iter<value_t>, iter<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_common_range
        : public _mut_range<mut_fwd_iter<value_t>, mut_fwd_iter<value_t>, value_t>
    {};
}
