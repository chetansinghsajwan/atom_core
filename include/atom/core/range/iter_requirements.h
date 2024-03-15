#pragma once
#include "atom/core/core/requirements.h"
#include "atom/core/core.h"

// clang-format off

namespace atom
{
    template <typename iter_t, typename value_t>
    concept _is_iter_of = requires
    {
        requires is_convertible_to<typename iter_t::value_t, value_t>;
    };

    template <typename iter_t>
    concept _is_mut_iter = requires(iter_t it)
    {
        { it.mut_value() } -> is_convertible_to<typename iter_t::value_t&>;
    };

    template <typename iter_t, typename value_t>
    concept _is_mut_iter_of = requires
    {
        requires is_same_as<typename iter_t::value_t, value_t>;
        requires _is_mut_iter<iter_t>;
    };
}

namespace atom
{
    template <typename iter_t, typename iter_end_t>
    concept is_iter_with_end = requires(iter_t it, iter_end_t it_end)
    {
        requires is_copyable<iter_end_t>;
        requires is_moveable<iter_end_t>;
        requires is_destructible<iter_end_t>;

        { it.is_eq(it_end) } -> is_same_as<bool>;
    };

    template <typename iter_t>
    concept is_iter = requires(iter_t it, const iter_t cit)
    {
        typename iter_t::value_t;

        requires is_moveable<iter_t>;
        requires is_destructible<iter_t>;

        { cit.value() } -> is_convertible_to<const typename iter_t::value_t&>;
        { it.next() } -> is_convertible_to<iter_t&>;
    };

    template <typename iter_t>
    concept is_mut_iter = requires
    {
        requires is_iter<iter_t>;
        requires _is_mut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_iter_of = requires
    {
        requires is_iter<iter_t>;
        requires _is_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_mut_iter_of = requires
    {
        requires is_mut_iter<iter_t>;
        requires _is_mut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_iter_pair = requires
    {
        requires is_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_mut_iter_pair = requires
    {
        requires is_mut_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_iter_pair_of = requires
    {
        requires is_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_iter_pair_of = requires
    {
        requires is_mut_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept is_fwd_iter = requires
    {
        requires is_iter<iter_t>;
        requires is_copyable<iter_t>;
    };

    template <typename iter_t>
    concept is_mut_fwd_iter = requires
    {
        requires is_fwd_iter<iter_t>;
        requires _is_mut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_fwd_iter_of = requires
    {
        requires is_fwd_iter<iter_t>;
        requires _is_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_mut_fwd_iter_of = requires
    {
        requires is_mut_fwd_iter<iter_t>;
        requires _is_mut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_fwd_iter_pair = requires
    {
        requires is_fwd_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_mut_fwd_iter_pair = requires
    {
        requires is_mut_fwd_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_fwd_iter_pair_of = requires
    {
        requires is_fwd_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_fwd_iter_pair_of = requires
    {
        requires is_mut_fwd_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept is_bidi_iter = requires(iter_t it)
    {
        requires is_fwd_iter<iter_t>;

        { it.prev() } -> is_convertible_to<iter_t&>;
    };

    template <typename iter_t>
    concept is_mut_bidi_iter = requires
    {
        requires is_bidi_iter<iter_t>;
        requires _is_mut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_bidi_iter_of = requires
    {
        requires is_bidi_iter<iter_t>;
        requires _is_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_mut_bidi_iter_of = requires
    {
        requires is_mut_bidi_iter<iter_t>;
        requires _is_mut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_bidi_iter_pair = requires
    {
        requires is_bidi_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_mut_bidi_iter_pair = requires
    {
        requires is_mut_bidi_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_bidi_iter_pair_of = requires
    {
        requires is_bidi_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_bidi_iter_pair_of = requires
    {
        requires is_mut_bidi_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept is_jump_iter = requires(iter_t it, const iter_t cit, usize steps)
    {
        requires is_bidi_iter<iter_t>;

        { it.next(steps) } -> is_convertible_to<iter_t&>;
        { it.prev(steps) } -> is_convertible_to<iter_t&>;
        { cit.compare(cit) } -> is_convertible_to<isize>;
    };

    template <typename iter_t>
    concept is_mut_jump_iter = requires
    {
        requires is_jump_iter<iter_t>;
        requires _is_mut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_jump_iter_of = requires
    {
        requires is_jump_iter<iter_t>;
        requires _is_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_mut_jump_iter_of = requires
    {
        requires is_mut_jump_iter<iter_t>;
        requires _is_mut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_jump_iter_pair = requires
    {
        requires is_jump_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_mut_jump_iter_pair = requires
    {
        requires is_mut_jump_iter<iter_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_jump_iter_pair_of = requires
    {
        requires is_jump_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_jump_iter_pair_of = requires
    {
        requires is_mut_jump_iter_of<iter_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept is_array_iter = requires(const iter_t cit)
    {
        requires is_jump_iter<iter_t>;

        { cit.get_data() } -> is_convertible_to<const typename iter_t::value_t*>;
    };

    template <typename iter_t>
    concept is_mut_array_iter = requires(iter_t it)
    {
        requires is_array_iter<iter_t>;
        requires _is_mut_iter<iter_t>;

        { it.get_mut_data() } -> is_convertible_to<typename iter_t::value_t*>;
    };

    template <typename iter_t, typename value_t>
    concept is_array_iter_of = requires
    {
        requires is_array_iter<iter_t>;
        requires _is_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept is_mut_array_iter_of = requires
    {
        requires is_mut_array_iter<iter_t>;
        requires _is_mut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_array_iter_pair = requires
    {
        requires is_array_iter<iter_t>;
        requires is_array_iter<iter_end_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept is_mut_array_iter_pair = requires
    {
        requires is_mut_array_iter<iter_t>;
        requires is_mut_array_iter<iter_end_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_array_iter_pair_of = requires
    {
        requires is_array_iter_of<iter_t, value_t>;
        requires is_array_iter_of<iter_end_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept is_mut_array_iter_pair_of = requires
    {
        requires is_mut_array_iter_of<iter_t, value_t>;
        requires is_mut_array_iter_of<iter_end_t, value_t>;
        requires is_iter_with_end<iter_t, iter_end_t>;
    };
}

// clang-format on
