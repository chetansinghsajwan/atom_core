export module atom_core:ranges.range_mocks;

import :ranges.iterator_mocks;

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iterator_type, typename in_iterator_end_type, typename in_elem_type>
    class _range
    {
    public:
        using value_type = in_elem_type;
        using iterator_type = in_iterator_type;
        using iterator_end_type = in_iterator_end_type;

    public:
        auto get_iterator() const -> iterator_type;
        auto get_iterator_end() const -> iterator_end_type;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iterator_type, typename in_iterator_end_type, typename in_elem_type>
    class _mut_range
    {
    public:
        using value_type = in_elem_type;
        using iterator_type = in_iterator_type;
        using iterator_end_type = in_iterator_end_type;
        using mut_iterator_type = in_iterator_type;
        using mut_iterator_end_type = in_iterator_end_type;

    public:
        auto get_iterator() const -> iterator_type;
        auto get_iterator_end() const -> iterator_end_type;
        auto get_mut_iterator() -> mut_iterator_type;
        auto get_mut_iterator_end() -> mut_iterator_end_type;
    };
}

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `is_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class range: public _range<iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_range: public _mut_range<mut_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class fwd_range: public _range<unidirectional_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_fwd_range
        : public _mut_range<mut_unidirectional_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class bidi_range: public _range<bidirectional_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_bidi_range
        : public _mut_range<mut_bidirectional_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_random_access_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class random_access_range
        : public _range<random_access_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_random_access_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_random_access_range
        : public _mut_range<mut_random_access_iterator<value_type>, iterator_end, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class array_range
        : public _range<array_iterator<value_type>, array_iterator<value_type>, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_array_range
        : public _mut_range<mut_array_iterator<value_type>, mut_array_iterator<value_type>,
              value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class common_range: public _range<iterator<value_type>, iterator<value_type>, value_type>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class mut_common_range
        : public _mut_range<mut_unidirectional_iterator<value_type>,
              mut_unidirectional_iterator<value_type>, value_type>
    {};
}
