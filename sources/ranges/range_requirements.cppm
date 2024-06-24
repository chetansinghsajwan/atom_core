export module atom.core:ranges.range_requirements;

import std;
import :types;
import :ranges.iterator_requirements;

namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept _is_range = requires(const range_type& range) {
        typename range_type::value_type;
        typename range_type::iterator_type;
        typename range_type::iterator_end_type;

        requires std::same_as<typename range_type::value_type,
            typename range_type::iterator_type::value_type>;

        { range.get_iterator() } -> std::same_as<typename range_type::iterator_type>;
        { range.get_iterator_end() } -> std::same_as<typename range_type::iterator_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept _is_range_of =
        _is_range<range_type> and std::convertible_to<typename range_type::value_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept _is_mut_range = requires(range_type& range) {
        requires _is_range<range_type>;

        typename range_type::mut_iterator_type;
        typename range_type::mut_iterator_end_type;

        { range.get_mut_iterator() } -> std::same_as<typename range_type::mut_iterator_type>;
        {
            range.get_mut_iterator_end()
        } -> std::same_as<typename range_type::mut_iterator_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept _is_mut_range_of = _is_mut_range<range_type>
                               and std::convertible_to<typename range_type::value_type, value_type>;
}

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_range = _is_range<range_type>
                       and is_iterator_pair<typename range_type::iterator_type,
                           typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_range_of = _is_range_of<range_type, value_type>
                          and is_iterator_pair_of<typename range_type::iterator_type,
                              typename range_type::iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_range = _is_mut_range<range_type>
                           and is_iterator_pair<typename range_type::iterator_type,
                               typename range_type::iterator_end_type>
                           and is_mut_iterator_pair<typename range_type::mut_iterator_type,
                               typename range_type::mut_iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_range_of = _is_mut_range_of<range_type, value_type>
                              and is_iterator_pair_of<typename range_type::iterator_type,
                                  typename range_type::iterator_end_type, value_type>
                              and is_mut_iterator_pair_of<typename range_type::mut_iterator_type,
                                  typename range_type::mut_iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `unidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_unidirectional_range =
        _is_range<range_type>
        and is_unidirectional_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `unidirectional_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_unidirectional_range_of =
        _is_range_of<range_type, value_type>
        and is_unidirectional_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_unidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_unidirectional_range =
        _is_mut_range<range_type>
        and is_unidirectional_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>
        and is_mut_unidirectional_iterator_pair<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_unidirectional_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_unidirectional_range_of =
        _is_mut_range_of<range_type, value_type>
        and is_unidirectional_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>
        and is_mut_unidirectional_iterator_pair_of<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `bidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_bidirectional_range =
        _is_range<range_type>
        and is_bidirectional_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `bidirectional_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_bidirectional_range_of =
        _is_range_of<range_type, value_type>
        and is_bidirectional_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_bidirectional_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_bidirectional_range =
        _is_mut_range<range_type>
        and is_bidirectional_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>
        and is_mut_bidirectional_iterator_pair<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_bidirectional_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_bidirectional_range_of =
        _is_mut_range_of<range_type, value_type>
        and is_bidirectional_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>
        and is_mut_bidirectional_iterator_pair_of<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `random_access_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_random_access_range =
        _is_range<range_type>
        and is_random_access_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `random_access_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_random_access_range_of =
        _is_range_of<range_type, value_type>
        and is_random_access_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_random_access_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_random_access_range =
        _is_mut_range<range_type>
        and is_random_access_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>
        and is_mut_random_access_iterator_pair<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_random_access_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_random_access_range_of =
        _is_mut_range_of<range_type, value_type>
        and is_random_access_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>
        and is_mut_random_access_iterator_pair_of<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_array_range = _is_range<range_type>
                             and is_array_iterator_pair<typename range_type::iterator_type,
                                 typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `array_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_array_range_of = _is_range_of<range_type, value_type>
                                and is_array_iterator_pair_of<typename range_type::iterator_type,
                                    typename range_type::iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_array_range =
        _is_mut_range<range_type>
        and is_array_iterator_pair<typename range_type::iterator_type,
            typename range_type::iterator_end_type>
        and is_mut_array_iterator_pair<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_array_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_array_range_of =
        _is_mut_range_of<range_type, value_type>
        and is_array_iterator_pair_of<typename range_type::iterator_type,
            typename range_type::iterator_end_type, value_type>
        and is_mut_array_iterator_pair_of<typename range_type::mut_iterator_type,
            typename range_type::mut_iterator_end_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename>
    concept is_rev_range = requires { requires is_bidirectional_range<range_type>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `rev_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_rev_range_of =
        requires { requires is_bidirectional_range_of<range_type, value_type>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_rev_range = requires { requires is_mut_bidirectional_range<range_type>; };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_rev_range` of type `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename value_type>
    concept is_mut_rev_range_of = is_mut_bidirectional_range_of<range_type, value_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_common_range =
        _is_range<range_type>
        and type_info<typename range_type::iterator_type>::template is_same_as<
            typename range_type::iterator_end_type>;

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept is_mut_common_range =
        is_common_range<range_type>
        and type_info<typename range_type::mut_iterator_type>::template is_same_as<
            typename range_type::mut_iterator_end_type>;
}
