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
        using elem_t = in_elem_t;
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
        using elem_t = in_elem_t;
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
    /// `rrange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class range: public _range<iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_range: public _mut_range<mut_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rfwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class fwd_range: public _range<fwd_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_fwd_range: public _mut_range<mut_fwd_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rbidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class bidi_range: public _range<bidi_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_bidi_range: public _mut_range<mut_bidi_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rjump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class jump_range: public _range<jump_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_jump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_jump_range: public _mut_range<mut_jump_iter<elem_t>, iter_end, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rarray_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class array_range: public _range<array_iter<elem_t>, array_iter<elem_t>, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_array_range
        : public _mut_range<mut_array_iter<elem_t>, mut_array_iter<elem_t>, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class common_range: public _range<iter<elem_t>, iter<elem_t>, elem_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_t>
    class mut_common_range
        : public _mut_range<mut_fwd_iter<elem_t>, mut_fwd_iter<elem_t>, elem_t>
    {};
}
