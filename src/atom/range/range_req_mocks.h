#pragma once
#include "iter_req_mocks.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename _titer, typename _titer_end, typename type>
    class _range_req_mock
    {
    public:
        using elem_type = type;
        using iter_type = _titer;
        using iter_end_type = _titer_end;

    public:
        auto iter() const -> iter_type;
        auto iter_end() const -> iter_end_type;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename _titer, typename _titer_end, typename type>
    class _mut_range_req_mock
    {
    public:
        using elem_type = type;
        using iter_type = _titer;
        using iter_end_type = _titer_end;
        using mut_iter_type = _titer;
        using mut_iter_end_type = _titer_end;

    public:
        auto iter() const -> iter_type;
        auto iter_end() const -> iter_end_type;
        auto mut_iter() -> mut_iter_type;
        auto mut_iter_end() -> mut_iter_end_type;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rrange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class range_req_mock: public _range_req_mock<iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_range_req_mock: public _mut_range_req_mock<mut_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rfwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class fwd_range_req_mock: public _range_req_mock<fwd_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_fwd_range_req_mock: public _mut_range_req_mock<mut_fwd_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rbidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class bidi_range_req_mock: public _range_req_mock<bidi_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_bidi_range_req_mock: public _mut_range_req_mock<mut_bidi_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rjump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class jump_range_req_mock: public _range_req_mock<jump_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_jump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_jump_range_req_mock: public _mut_range_req_mock<mut_jump_iter_req_mock<type>, iter_end_req_mock, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rarray_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class array_range_req_mock: public _range_req_mock<array_iter_req_mock<type>, array_iter_req_mock<type>, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_array_range_req_mock
        : public _mut_range_req_mock<mut_array_iter_req_mock<type>, mut_array_iter_req_mock<type>, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class common_range_req_mock: public _range_req_mock<iter_req_mock<type>, iter_req_mock<type>, type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class mut_common_range_req_mock
        : public _mut_range_req_mock<mut_fwd_iter_req_mock<type>, mut_fwd_iter_req_mock<type>, type>
    {};
}
