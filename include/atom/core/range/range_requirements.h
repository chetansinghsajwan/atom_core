#pragma once
#include "atom/core/core/requirements.h"
#include "atom/core/range/iter_requirements.h"

// clang-format off
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _rrange = requires(const range_t& range)
    {
        typename range_t::elem_t;
        typename range_t::iter_t;
        typename range_t::iter_end_t;

        requires rsame_as<typename range_t::elem_t, typename range_t::iter_t::elem_t>;

        { range.get_iter() } -> rsame_as<typename range_t::iter_t>;
        { range.get_iter_end() } -> rsame_as<typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _rrange_of = requires(const range_t& range)
    {
        requires _rrange<range_t>;
        requires rconvertible_to<typename range_t::elem_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _rmut_range = requires(range_t& range)
    {
        requires _rrange<range_t>;

        typename range_t::mut_iter_t;
        typename range_t::mut_iter_end_t;

        { range.get_mut_iter() } -> rsame_as<typename range_t::mut_iter_t>;
        { range.get_mut_iter_end() } -> rsame_as<typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _rmut_range_of = requires(range_t& range)
    {
        requires _rmut_range<range_t>;
        requires rconvertible_to<typename range_t::elem_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rrange = requires
    {
        requires _rrange<range_t>;
        requires riter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rrange_of = requires
    {
        requires _rrange_of<range_t, value_t>;
        requires riter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_range = requires
    {
        requires _rmut_range<range_t>;
        requires riter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires rmut_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_range_of = requires
    {
        requires _rmut_range_of<range_t, value_t>;
        requires riter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires rmut_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rfwd_range = requires
    {
        requires _rrange<range_t>;
        requires rfwd_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `fwd_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rfwd_range_of = requires
    {
        requires _rrange_of<range_t, value_t>;
        requires rfwd_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_fwd_range = requires
    {
        requires _rmut_range<range_t>;
        requires rfwd_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires rmut_fwd_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_fwd_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_fwd_range_of = requires
    {
        requires _rmut_range_of<range_t, value_t>;
        requires rfwd_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires rmut_fwd_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rbidi_range = requires
    {
        requires _rrange<range_t>;
        requires rbidi_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidi_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rbidi_range_of = requires
    {
        requires _rrange_of<range_t, value_t>;
        requires rbidi_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_bidi_range = requires
    {
        requires _rmut_range<range_t>;
        requires rbidi_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires rmut_bidi_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidi_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_bidi_range_of = requires
    {
        requires _rmut_range_of<range_t, value_t>;
        requires rbidi_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires rmut_bidi_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rjump_range = requires
    {
        requires _rrange<range_t>;
        requires rjump_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `jump_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rjump_range_of = requires
    {
        requires _rrange_of<range_t, value_t>;
        requires rjump_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_jump_range = requires
    {
        requires _rmut_range<range_t>;
        requires rjump_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires rmut_jump_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_jump_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_jump_range_of = requires
    {
        requires _rmut_range_of<range_t, value_t>;
        requires rjump_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires rmut_jump_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rarray_range = requires
    {
        requires _rrange<range_t>;
        requires rarray_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rarray_range_of = requires
    {
        requires _rrange_of<range_t, value_t>;
        requires rarray_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_array_range = requires
    {
        requires _rmut_range<range_t>;
        requires rarray_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires rmut_array_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_array_range_of = requires
    {
        requires _rmut_range_of<range_t, value_t>;
        requires rarray_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires rmut_array_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename>
    concept rrev_range = requires
    {
        requires rbidi_range<range_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rrev_range_of = requires
    {
        requires rbidi_range_of<range_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_rev_range = requires
    {
        requires rmut_bidi_range<range_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept rmut_rev_range_of = requires
    {
        requires rmut_bidi_range_of<range_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rcommon_range = requires
    {
        requires _rrange<range_t>;
        requires rsame_as<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept rmut_common_range = requires
    {
        requires rcommon_range<range_t>;
        requires rsame_as<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };
}
