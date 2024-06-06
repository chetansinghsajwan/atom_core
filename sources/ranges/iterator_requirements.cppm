export module atom.core:ranges.iterator_requirements;

import std;
import :core;

namespace atom::ranges
{
    template <typename iterator_type, typename value_type>
    concept _is_iterator_of = std::same_as<typename iterator_type::value_type, value_type>;

    template <typename iterator_type>
    concept _is_mut_iterator = requires(iterator_type it) {
        { *it } -> std::same_as<typename iterator_type::value_type&>;
    };

    template <typename iterator_type, typename value_type>
    concept _is_mut_iterator_of =
        _is_iterator_of<iterator_type, value_type> and _is_mut_iterator<iterator_type>;
}

export namespace atom::ranges
{
    template <typename iterator_type, typename iterator_end_type>
    concept is_iterator_with_end = requires(iterator_type begin, iterator_end_type end) {
        requires std::copyable<iterator_end_type>;
        requires std::movable<iterator_end_type>;
        requires std::destructible<iterator_end_type>;

        { begin == end } -> std::same_as<bool>;
        { begin != end } -> std::same_as<bool>;
    };

    template <typename iterator_type>
    concept is_iterator = std::input_iterator<iterator_type>;

    template <typename iterator_type>
    concept is_mut_iterator = is_iterator<iterator_type> and _is_mut_iterator<iterator_type>;

    template <typename iterator_type, typename value_type>
    concept is_iterator_of =
        is_iterator<iterator_type> and _is_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename value_type>
    concept is_mut_iterator_of =
        is_mut_iterator<iterator_type> and _is_mut_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_iterator_pair =
        is_iterator<iterator_type> and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_mut_iterator_pair =
        is_mut_iterator<iterator_type> and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_iterator_pair_of = is_iterator_of<iterator_type, value_type>
                                  and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_mut_iterator_pair_of = is_mut_iterator_of<iterator_type, value_type>
                                      and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type>
    concept is_unidirectional_iterator =
        is_iterator<iterator_type> and std::copyable<iterator_type>;

    template <typename iterator_type>
    concept is_mut_unidirectional_iterator =
        is_unidirectional_iterator<iterator_type> and _is_mut_iterator<iterator_type>;

    template <typename iterator_type, typename value_type>
    concept is_unidirectional_iterator_of =
        is_unidirectional_iterator<iterator_type> and _is_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename value_type>
    concept is_mut_unidirectional_iterator_of = is_mut_unidirectional_iterator<iterator_type>
                                                and _is_mut_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_unidirectional_iterator_pair =
        is_unidirectional_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_mut_unidirectional_iterator_pair =
        is_mut_unidirectional_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_unidirectional_iterator_pair_of =
        is_unidirectional_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_mut_unidirectional_iterator_pair_of =
        is_mut_unidirectional_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type>
    concept is_bidirectional_iterator = std::bidirectional_iterator<iterator_type>;

    template <typename iterator_type>
    concept is_mut_bidirectional_iterator =
        is_bidirectional_iterator<iterator_type> and _is_mut_iterator<iterator_type>;

    template <typename iterator_type, typename value_type>
    concept is_bidirectional_iterator_of =
        is_bidirectional_iterator<iterator_type> and _is_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename value_type>
    concept is_mut_bidirectional_iterator_of = is_mut_bidirectional_iterator<iterator_type>
                                               and _is_mut_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_bidirectional_iterator_pair =
        is_bidirectional_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_mut_bidirectional_iterator_pair =
        is_mut_bidirectional_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_bidirectional_iterator_pair_of =
        is_bidirectional_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_mut_bidirectional_iterator_pair_of =
        is_mut_bidirectional_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type>
    concept is_random_access_iterator = std::random_access_iterator<iterator_type>;

    template <typename iterator_type>
    concept is_mut_random_access_iterator =
        is_random_access_iterator<iterator_type> and _is_mut_iterator<iterator_type>;

    template <typename iterator_type, typename value_type>
    concept is_random_access_iterator_of =
        is_random_access_iterator<iterator_type> and _is_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename value_type>
    concept is_mut_random_access_iterator_of = is_mut_random_access_iterator<iterator_type>
                                               and _is_mut_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_random_access_iterator_pair =
        is_random_access_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_mut_random_access_iterator_pair =
        is_mut_random_access_iterator<iterator_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_random_access_iterator_pair_of =
        is_random_access_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_mut_random_access_iterator_pair_of =
        is_mut_random_access_iterator_of<iterator_type, value_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type>
    concept is_array_iterator = std::contiguous_iterator<iterator_type>;

    template <typename iterator_type>
    concept is_mut_array_iterator =
        is_array_iterator<iterator_type> and _is_mut_iterator<iterator_type>;

    template <typename iterator_type, typename value_type>
    concept is_array_iterator_of =
        is_array_iterator<iterator_type> and _is_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename value_type>
    concept is_mut_array_iterator_of =
        is_mut_array_iterator<iterator_type> and _is_mut_iterator_of<iterator_type, value_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_array_iterator_pair =
        is_array_iterator<iterator_type> and is_array_iterator<iterator_end_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type>
    concept is_mut_array_iterator_pair =
        is_mut_array_iterator<iterator_type> and is_mut_array_iterator<iterator_end_type>
        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_array_iterator_pair_of = is_array_iterator_of<iterator_type, value_type>
                                        and is_array_iterator_of<iterator_end_type, value_type>
                                        and is_iterator_with_end<iterator_type, iterator_end_type>;

    template <typename iterator_type, typename iterator_end_type, typename value_type>
    concept is_mut_array_iterator_pair_of = is_mut_array_iterator_of<iterator_type, value_type>
                                            and is_mut_array_iterator_of<iterator_end_type, value_type>
                                            and is_iterator_with_end<iterator_type, iterator_end_type>;
}
