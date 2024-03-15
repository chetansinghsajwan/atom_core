#pragma once
#include "atom/core/core/requirements.h"
#include "atom/core/core.h"

// clang-format off

namespace atom
{
    template <typename iter_t, typename value_t>
    concept _riter_of = requires
    {
        requires rconvertible_to<typename iter_t::elem_t, value_t>;
    };

    template <typename iter_t>
    concept _rmut_iter = requires(iter_t it)
    {
        { it.mut_value() } -> rconvertible_to<typename iter_t::elem_t&>;
    };

    template <typename iter_t, typename value_t>
    concept _rmut_iter_of = requires
    {
        requires rsame_as<typename iter_t::elem_t, value_t>;
        requires _rmut_iter<iter_t>;
    };
}

namespace atom
{
    template <typename iter_t, typename iter_end_t>
    concept riter_with_end = requires(iter_t it, iter_end_t it_end)
    {
        requires rcopyable<iter_end_t>;
        requires rmoveable<iter_end_t>;
        requires rdestructible<iter_end_t>;

        { it.is_eq(it_end) } -> rsame_as<bool>;
    };

    template <typename iter_t>
    concept riter = requires(iter_t it, const iter_t cit)
    {
        typename iter_t::elem_t;

        requires rmoveable<iter_t>;
        requires rdestructible<iter_t>;

        { cit.value() } -> rconvertible_to<const typename iter_t::elem_t&>;
        { it.next() } -> rconvertible_to<iter_t&>;
    };

    template <typename iter_t>
    concept rmut_iter = requires
    {
        requires riter<iter_t>;
        requires _rmut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept riter_of = requires
    {
        requires riter<iter_t>;
        requires _riter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept rmut_iter_of = requires
    {
        requires rmut_iter<iter_t>;
        requires _rmut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept riter_pair = requires
    {
        requires riter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rmut_iter_pair = requires
    {
        requires rmut_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept riter_pair_of = requires
    {
        requires riter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rmut_iter_pair_of = requires
    {
        requires rmut_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept rfwd_iter = requires
    {
        requires riter<iter_t>;
        requires rcopyable<iter_t>;
    };

    template <typename iter_t>
    concept rmut_fwd_iter = requires
    {
        requires rfwd_iter<iter_t>;
        requires _rmut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept rfwd_iter_of = requires
    {
        requires rfwd_iter<iter_t>;
        requires _riter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept rmut_fwd_iter_of = requires
    {
        requires rmut_fwd_iter<iter_t>;
        requires _rmut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rfwd_iter_pair = requires
    {
        requires rfwd_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rmut_fwd_iter_pair = requires
    {
        requires rmut_fwd_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rfwd_iter_pair_of = requires
    {
        requires rfwd_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rmut_fwd_iter_pair_of = requires
    {
        requires rmut_fwd_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept rbidi_iter = requires(iter_t it)
    {
        requires rfwd_iter<iter_t>;

        { it.prev() } -> rconvertible_to<iter_t&>;
    };

    template <typename iter_t>
    concept rmut_bidi_iter = requires
    {
        requires rbidi_iter<iter_t>;
        requires _rmut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept rbidi_iter_of = requires
    {
        requires rbidi_iter<iter_t>;
        requires _riter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept rmut_bidi_iter_of = requires
    {
        requires rmut_bidi_iter<iter_t>;
        requires _rmut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rbidi_iter_pair = requires
    {
        requires rbidi_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rmut_bidi_iter_pair = requires
    {
        requires rmut_bidi_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rbidi_iter_pair_of = requires
    {
        requires rbidi_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rmut_bidi_iter_pair_of = requires
    {
        requires rmut_bidi_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept rjump_iter = requires(iter_t it, const iter_t cit, usize steps)
    {
        requires rbidi_iter<iter_t>;

        { it.next(steps) } -> rconvertible_to<iter_t&>;
        { it.prev(steps) } -> rconvertible_to<iter_t&>;
        { cit.compare(cit) } -> rconvertible_to<isize>;
    };

    template <typename iter_t>
    concept rmut_jump_iter = requires
    {
        requires rjump_iter<iter_t>;
        requires _rmut_iter<iter_t>;
    };

    template <typename iter_t, typename value_t>
    concept rjump_iter_of = requires
    {
        requires rjump_iter<iter_t>;
        requires _riter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept rmut_jump_iter_of = requires
    {
        requires rmut_jump_iter<iter_t>;
        requires _rmut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rjump_iter_pair = requires
    {
        requires rjump_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rmut_jump_iter_pair = requires
    {
        requires rmut_jump_iter<iter_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rjump_iter_pair_of = requires
    {
        requires rjump_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rmut_jump_iter_pair_of = requires
    {
        requires rmut_jump_iter_of<iter_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t>
    concept rarray_iter = requires(const iter_t cit)
    {
        requires rjump_iter<iter_t>;

        { cit.get_data() } -> rconvertible_to<const typename iter_t::elem_t*>;
    };

    template <typename iter_t>
    concept rmut_array_iter = requires(iter_t it)
    {
        requires rarray_iter<iter_t>;
        requires _rmut_iter<iter_t>;

        { it.get_mut_data() } -> rconvertible_to<typename iter_t::elem_t*>;
    };

    template <typename iter_t, typename value_t>
    concept rarray_iter_of = requires
    {
        requires rarray_iter<iter_t>;
        requires _riter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename value_t>
    concept rmut_array_iter_of = requires
    {
        requires rmut_array_iter<iter_t>;
        requires _rmut_iter_of<iter_t, value_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rarray_iter_pair = requires
    {
        requires rarray_iter<iter_t>;
        requires rarray_iter<iter_end_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t>
    concept rmut_array_iter_pair = requires
    {
        requires rmut_array_iter<iter_t>;
        requires rmut_array_iter<iter_end_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rarray_iter_pair_of = requires
    {
        requires rarray_iter_of<iter_t, value_t>;
        requires rarray_iter_of<iter_end_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };

    template <typename iter_t, typename iter_end_t, typename value_t>
    concept rmut_array_iter_pair_of = requires
    {
        requires rmut_array_iter_of<iter_t, value_t>;
        requires rmut_array_iter_of<iter_end_t, value_t>;
        requires riter_with_end<iter_t, iter_end_t>;
    };
}

// clang-format on
