export module atom.core:ranges.range_requirements;

import std;
import :types;
import :ranges.iterator_requirements;

namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _is_range = requires(const range_t& range) {
        typename range_t::value_t;
        typename range_t::iterator_t;
        typename range_t::iterator_end_t;

        requires std::same_as<typename range_t::value_t, typename range_t::iterator_t::value_t>;

        { range.get_iterator() } -> std::same_as<typename range_t::iterator_t>;
        { range.get_iterator_end() } -> std::same_as<typename range_t::iterator_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _is_range_of =
        _is_range<range_t> and std::convertible_to<typename range_t::value_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _is_mut_range = requires(range_t& range) {
        requires _is_range<range_t>;

        typename range_t::mut_iterator_t;
        typename range_t::mut_iterator_end_t;

        { range.get_mut_iterator() } -> std::same_as<typename range_t::mut_iterator_t>;
        { range.get_mut_iterator_end() } -> std::same_as<typename range_t::mut_iterator_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _is_mut_range_of =
        _is_mut_range<range_t> and std::convertible_to<typename range_t::value_t, value_t>;
}

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_range =
        _is_range<range_t>
        and is_iterator_pair<typename range_t::iterator_t, typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_range_of = _is_range_of<range_t, value_t>
                          and is_iterator_pair_of<typename range_t::iterator_t,
                              typename range_t::iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_range =
        _is_mut_range<range_t>
        and is_iterator_pair<typename range_t::iterator_t, typename range_t::iterator_end_t>
        and is_mut_iterator_pair<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_range_of = _is_mut_range_of<range_t, value_t>
                              and is_iterator_pair_of<typename range_t::iterator_t,
                                  typename range_t::iterator_end_t, value_t>
                              and is_mut_iterator_pair_of<typename range_t::mut_iterator_t,
                                  typename range_t::mut_iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `unidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_unidirectional_range =
        _is_range<range_t>
        and is_unidirectional_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `unidirectional_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_unidirectional_range_of =
        _is_range_of<range_t, value_t>
        and is_unidirectional_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_unidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_unidirectional_range =
        _is_mut_range<range_t>
        and is_unidirectional_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>
        and is_mut_unidirectional_iterator_pair<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_unidirectional_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_unidirectional_range_of =
        _is_mut_range_of<range_t, value_t>
        and is_unidirectional_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>
        and is_mut_unidirectional_iterator_pair_of<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_bidirectional_range =
        _is_range<range_t>
        and is_bidirectional_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidirectional_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_bidirectional_range_of =
        _is_range_of<range_t, value_t>
        and is_bidirectional_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_bidirectional_range =
        _is_mut_range<range_t>
        and is_bidirectional_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>
        and is_mut_bidirectional_iterator_pair<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidirectional_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_bidirectional_range_of =
        _is_mut_range_of<range_t, value_t>
        and is_bidirectional_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>
        and is_mut_bidirectional_iterator_pair_of<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `random_access_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_random_access_range =
        _is_range<range_t>
        and is_random_access_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `random_access_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_random_access_range_of =
        _is_range_of<range_t, value_t>
        and is_random_access_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_random_access_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_random_access_range =
        _is_mut_range<range_t>
        and is_random_access_iterator_pair<typename range_t::iterator_t,
            typename range_t::iterator_end_t>
        and is_mut_random_access_iterator_pair<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_random_access_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_random_access_range_of =
        _is_mut_range_of<range_t, value_t>
        and is_random_access_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>
        and is_mut_random_access_iterator_pair_of<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_array_range =
        _is_range<range_t>
        and is_array_iterator_pair<typename range_t::iterator_t, typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_array_range_of = _is_range_of<range_t, value_t>
                                and is_array_iterator_pair_of<typename range_t::iterator_t,
                                    typename range_t::iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_array_range =
        _is_mut_range<range_t>
        and is_array_iterator_pair<typename range_t::iterator_t, typename range_t::iterator_end_t>
        and is_mut_array_iterator_pair<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_array_range_of =
        _is_mut_range_of<range_t, value_t>
        and is_array_iterator_pair_of<typename range_t::iterator_t,
            typename range_t::iterator_end_t, value_t>
        and is_mut_array_iterator_pair_of<typename range_t::mut_iterator_t,
            typename range_t::mut_iterator_end_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename>
    concept is_rev_range = requires { requires is_bidirectional_range<range_t>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_rev_range_of = requires { requires is_bidirectional_range_of<range_t, value_t>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_rev_range = requires { requires is_mut_bidirectional_range<range_t>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_rev_range_of = is_mut_bidirectional_range_of<range_t, value_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_common_range = _is_range<range_t>
                              and typeinfo<typename range_t::iterator_t>::template is_same_as<
                                  typename range_t::iterator_end_t>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_common_range =
        is_common_range<range_t>
        and typeinfo<typename range_t::mut_iterator_t>::template is_same_as<
            typename range_t::mut_iterator_end_t>;
}
