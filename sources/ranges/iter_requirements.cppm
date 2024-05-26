export module atom.core:ranges.iter_requirements;

import std;
import :core;

namespace atom
{
    template <typename iter_t, typename value_t>
    concept _is_iter_of = std::same_as<typename iter_t::value_t, value_t>;

    template <typename iter_t>
    concept _is_mut_iter = requires(iter_t it) {
        { *it } -> std::same_as<typename iter_t::value_t&>;
    };

    template <typename iter_t, typename value_t>
    concept _is_mut_iter_of = _is_iter_of<iter_t, value_t> and _is_mut_iter<iter_t>;
}

export namespace atom
{
    template <typename iter_t, typename iter_end_t>
    concept is_iter_with_end = requires(iter_t begin, iter_end_t end) {
        requires std::copyable<iter_end_t>;
        requires std::movable<iter_end_t>;
        requires std::destructible<iter_end_t>;

        { begin == end } -> std::same_as<bool>;
        { begin != end } -> std::same_as<bool>;
    };

    template <typename iter_t>
    concept is_iter = std::input_iterator<iter_t>;

    template <typename iter_t>
    concept is_mut_iter = is_iter<iter_t> and _is_mut_iter<iter_t>;

    template <typename iter_t, typename value_t>
    concept is_iter_of = is_iter<iter_t> and _is_iter_of<iter_t, value_t>;

    template <typename iter_t, typename value_t>
    concept is_mut_iter_of = is_mut_iter<iter_t> and _is_mut_iter_of<iter_t, value_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_iter_pair = is_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_mut_iter_pair = is_mut_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_iter_pair_of = is_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_iter_pair_of =
        is_mut_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t>
    concept is_fwd_iter = is_iter<iter_t> and std::copyable<iter_t>;

    template <typename iter_t>
    concept is_mut_fwd_iter = is_fwd_iter<iter_t> and _is_mut_iter<iter_t>;

    template <typename iter_t, typename value_t>
    concept is_fwd_iter_of = is_fwd_iter<iter_t> and _is_iter_of<iter_t, value_t>;

    template <typename iter_t, typename value_t>
    concept is_mut_fwd_iter_of = is_mut_fwd_iter<iter_t> and _is_mut_iter_of<iter_t, value_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_fwd_iter_pair = is_fwd_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_mut_fwd_iter_pair = is_mut_fwd_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_fwd_iter_pair_of =
        is_fwd_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_fwd_iter_pair_of =
        is_mut_fwd_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t>
    concept is_bidi_iter = std::bidirectional_iterator<iter_t>;

    template <typename iter_t>
    concept is_mut_bidi_iter = is_bidi_iter<iter_t> and _is_mut_iter<iter_t>;

    template <typename iter_t, typename value_t>
    concept is_bidi_iter_of = is_bidi_iter<iter_t> and _is_iter_of<iter_t, value_t>;

    template <typename iter_t, typename value_t>
    concept is_mut_bidi_iter_of = is_mut_bidi_iter<iter_t> and _is_mut_iter_of<iter_t, value_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_bidi_iter_pair = is_bidi_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_mut_bidi_iter_pair =
        is_mut_bidi_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_bidi_iter_pair_of =
        is_bidi_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_bidi_iter_pair_of =
        is_mut_bidi_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t>
    concept is_jump_iter = std::random_access_iterator<iter_t>;

    template <typename iter_t>
    concept is_mut_jump_iter = is_jump_iter<iter_t> and _is_mut_iter<iter_t>;

    template <typename iter_t, typename value_t>
    concept is_jump_iter_of = is_jump_iter<iter_t> and _is_iter_of<iter_t, value_t>;

    template <typename iter_t, typename value_t>
    concept is_mut_jump_iter_of = is_mut_jump_iter<iter_t> and _is_mut_iter_of<iter_t, value_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_jump_iter_pair = is_jump_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_mut_jump_iter_pair =
        is_mut_jump_iter<iter_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_jump_iter_pair_of =
        is_jump_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_jump_iter_pair_of =
        is_mut_jump_iter_of<iter_t, value_t> and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t>
    concept is_array_iter = std::contiguous_iterator<iter_t>;

    template <typename iter_t>
    concept is_mut_array_iter = is_array_iter<iter_t> and _is_mut_iter<iter_t>;

    template <typename iter_t, typename value_t>
    concept is_array_iter_of = is_array_iter<iter_t> and _is_iter_of<iter_t, value_t>;

    template <typename iter_t, typename value_t>
    concept is_mut_array_iter_of = is_mut_array_iter<iter_t> and _is_mut_iter_of<iter_t, value_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_array_iter_pair = is_array_iter<iter_t> and is_array_iter<iter_end_t>
                                 and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t>
    concept is_mut_array_iter_pair = is_mut_array_iter<iter_t> and is_mut_array_iter<iter_end_t>
                                     and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_array_iter_pair_of =
        is_array_iter_of<iter_t, value_t> and is_array_iter_of<iter_end_t, value_t>
        and is_iter_with_end<iter_t, iter_end_t>;

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_array_iter_pair_of =
        is_mut_array_iter_of<iter_t, value_t> and is_mut_array_iter_of<iter_end_t, value_t>
        and is_iter_with_end<iter_t, iter_end_t>;
}
