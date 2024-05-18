export module atom.core:ranges.range_requirements;

import std;
import :ranges.iter_requirements;

// clang-format off
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _is_range = requires(const range_t& range)
    {
        typename range_t::value_t;
        typename range_t::iter_t;
        typename range_t::iter_end_t;

        requires std::same_as<typename range_t::value_t, typename range_t::iter_t::value_t>;

        { range.get_iter() } -> std::same_as<typename range_t::iter_t>;
        { range.get_iter_end() } -> std::same_as<typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _is_range_of = requires(const range_t& range)
    {
        requires _is_range<range_t>;
        requires std::convertible_to<typename range_t::value_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept _is_mut_range = requires(range_t& range)
    {
        requires _is_range<range_t>;

        typename range_t::mut_iter_t;
        typename range_t::mut_iter_end_t;

        { range.get_mut_iter() } -> std::same_as<typename range_t::mut_iter_t>;
        { range.get_mut_iter_end() } -> std::same_as<typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept _is_mut_range_of = requires(range_t& range)
    {
        requires _is_mut_range<range_t>;
        requires std::convertible_to<typename range_t::value_t, value_t>;
    };
}

export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_range = requires
    {
        requires _is_range<range_t>;
        requires is_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_range_of = requires
    {
        requires _is_range_of<range_t, value_t>;
        requires is_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_range = requires
    {
        requires _is_mut_range<range_t>;
        requires is_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires is_mut_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_range_of = requires
    {
        requires _is_mut_range_of<range_t, value_t>;
        requires is_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires is_mut_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_fwd_range = requires
    {
        requires _is_range<range_t>;
        requires is_fwd_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `fwd_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_fwd_range_of = requires
    {
        requires _is_range_of<range_t, value_t>;
        requires is_fwd_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_fwd_range = requires
    {
        requires _is_mut_range<range_t>;
        requires is_fwd_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires is_mut_fwd_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_fwd_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_fwd_range_of = requires
    {
        requires _is_mut_range_of<range_t, value_t>;
        requires is_fwd_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires is_mut_fwd_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_bidi_range = requires
    {
        requires _is_range<range_t>;
        requires is_bidi_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `bidi_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_bidi_range_of = requires
    {
        requires _is_range_of<range_t, value_t>;
        requires is_bidi_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_bidi_range = requires
    {
        requires _is_mut_range<range_t>;
        requires is_bidi_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires is_mut_bidi_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_bidi_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_bidi_range_of = requires
    {
        requires _is_mut_range_of<range_t, value_t>;
        requires is_bidi_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires is_mut_bidi_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_jump_range = requires
    {
        requires _is_range<range_t>;
        requires is_jump_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `jump_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_jump_range_of = requires
    {
        requires _is_range_of<range_t, value_t>;
        requires is_jump_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_jump_range = requires
    {
        requires _is_mut_range<range_t>;
        requires is_jump_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires is_mut_jump_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_jump_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_jump_range_of = requires
    {
        requires _is_mut_range_of<range_t, value_t>;
        requires is_jump_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires is_mut_jump_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_array_range = requires
    {
        requires _is_range<range_t>;
        requires is_array_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_array_range_of = requires
    {
        requires _is_range_of<range_t, value_t>;
        requires is_array_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_array_range = requires
    {
        requires _is_mut_range<range_t>;
        requires is_array_iter_pair<typename range_t::iter_t, typename range_t::iter_end_t>;
        requires is_mut_array_iter_pair<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_array_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_array_range_of = requires
    {
        requires _is_mut_range_of<range_t, value_t>;
        requires is_array_iter_pair_of<typename range_t::iter_t, typename range_t::iter_end_t, value_t>;
        requires is_mut_array_iter_pair_of<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename>
    concept is_rev_range = requires
    {
        requires is_bidi_range<range_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_rev_range_of = requires
    {
        requires is_bidi_range_of<range_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_rev_range = requires
    {
        requires is_mut_bidi_range<range_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_rev_range` of type `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename value_t>
    concept is_mut_rev_range_of = requires
    {
        requires is_mut_bidi_range_of<range_t, value_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_common_range = requires
    {
        requires _is_range<range_t>;
        requires std::same_as<typename range_t::iter_t, typename range_t::iter_end_t>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_t` is `mut_common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    concept is_mut_common_range = requires
    {
        requires is_common_range<range_t>;
        requires std::same_as<typename range_t::mut_iter_t, typename range_t::mut_iter_end_t>;
    };
}
