export module atom.core:range.range_mocks;
import :range.iter_mocks;

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iter_type, typename in_iter_end_type, typename in_elem_type>
    class _range
    {
    public:
        using elem_type = in_elem_type;
        using iter_type = in_iter_type;
        using iter_end_type = in_iter_end_type;

    public:
        auto get_iter() const -> iter_type;
        auto get_iter_end() const -> iter_end_type;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iter_type, typename in_iter_end_type, typename in_elem_type>
    class _mut_range
    {
    public:
        using elem_type = in_elem_type;
        using iter_type = in_iter_type;
        using iter_end_type = in_iter_end_type;
        using mut_iter_type = in_iter_type;
        using mut_iter_end_type = in_iter_end_type;

    public:
        auto get_iter() const -> iter_type;
        auto get_iter_end() const -> iter_end_type;
        auto get_mut_iter() -> mut_iter_type;
        auto get_mut_iter_end() -> mut_iter_end_type;
    };
}

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `rrange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class range: public _range<iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_range: public _mut_range<mut_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rfwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class fwd_range: public _range<fwd_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_fwd_range: public _mut_range<mut_fwd_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rbidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class bidi_range: public _range<bidi_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_bidi_range: public _mut_range<mut_bidi_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rjump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class jump_range: public _range<jump_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_jump_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_jump_range: public _mut_range<mut_jump_iter<elem_type>, iter_end, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rarray_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class array_range: public _range<array_iter<elem_type>, array_iter<elem_type>, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_array_range
        : public _mut_range<mut_array_iter<elem_type>, mut_array_iter<elem_type>, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class common_range: public _range<iter<elem_type>, iter<elem_type>, elem_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rmut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename elem_type>
    class mut_common_range
        : public _mut_range<mut_fwd_iter<elem_type>, mut_fwd_iter<elem_type>, elem_type>
    {};
}
