export module atom.core:ranges.range_mocks;

import :ranges.iterator_mocks;

namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iterator_t, typename in_iterator_end_t, typename in_elem_t>
    class _range
    {
    public:
        using value_t = in_elem_t;
        using iterator_t = in_iterator_t;
        using iterator_end_t = in_iterator_end_t;

    public:
        auto get_iterator() const -> iterator_t;
        auto get_iterator_end() const -> iterator_end_t;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename in_iterator_t, typename in_iterator_end_t, typename in_elem_t>
    class _mut_range
    {
    public:
        using value_t = in_elem_t;
        using iterator_t = in_iterator_t;
        using iterator_end_t = in_iterator_end_t;
        using mut_iterator_t = in_iterator_t;
        using mut_iterator_end_t = in_iterator_end_t;

    public:
        auto get_iterator() const -> iterator_t;
        auto get_iterator_end() const -> iterator_end_t;
        auto get_mut_iterator() -> mut_iterator_t;
        auto get_mut_iterator_end() -> mut_iterator_end_t;
    };
}

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// `is_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class range: public _range<iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_range: public _mut_range<mut_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class fwd_range: public _range<unidirectional_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_fwd_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_fwd_range: public _mut_range<mut_unidirectional_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class bidi_range: public _range<bidirectional_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidi_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_bidi_range: public _mut_range<mut_bidirectional_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_random_access_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class random_access_range: public _range<random_access_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_random_access_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_random_access_range: public _mut_range<mut_random_access_iterator<value_t>, iterator_end, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class array_range: public _range<array_iterator<value_t>, array_iterator<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_array_range
        : public _mut_range<mut_array_iterator<value_t>, mut_array_iterator<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `rcommon_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class common_range: public _range<iterator<value_t>, iterator<value_t>, value_t>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_common_range` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class mut_common_range: public _mut_range<mut_unidirectional_iterator<value_t>, mut_unidirectional_iterator<value_t>, value_t>
    {};
}
