export module atom.core:range.range_requirements;
import :range.iter_requirements;
import :requirements;

// clang-format off
export namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept _rrange = requires(const range_type& range)
    {
        typename range_type::elem_type;
        typename range_type::iter_type;
        typename range_type::iter_end_type;

        requires rsame_as<typename range_type::elem_type, typename range_type::iter_type::elem_type>;

        { range.iter() } -> rsame_as<typename range_type::iter_type>;
        { range.iter_end() } -> rsame_as<typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept _rrange_of = requires(const range_type& range)
    {
        requires _rrange<range_type>;
        requires rconvertible_to<typename range_type::elem_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept _rmut_range = requires(range_type& range)
    {
        requires _rrange<range_type>;

        typename range_type::mut_iter_type;
        typename range_type::mut_iter_end_type;

        { range.mut_iter() } -> rsame_as<typename range_type::mut_iter_type>;
        { range.mut_iter_end() } -> rsame_as<typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// basic mut range requirements.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept _rmut_range_of = requires(range_type& range)
    {
        requires _rmut_range<range_type>;
        requires rconvertible_to<typename range_type::elem_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rrange = requires
    {
        requires _rrange<range_type>;
        requires riter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rrange_of = requires
    {
        requires _rrange_of<range_type, type>;
        requires riter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_range = requires
    {
        requires _rmut_range<range_type>;
        requires riter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
        requires rmut_iter_pair<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_range_of = requires
    {
        requires _rmut_range_of<range_type, type>;
        requires riter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
        requires rmut_iter_pair_of<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rfwd_range = requires
    {
        requires _rrange<range_type>;
        requires rfwd_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `fwd_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rfwd_range_of = requires
    {
        requires _rrange_of<range_type, type>;
        requires rfwd_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_fwd_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_fwd_range = requires
    {
        requires _rmut_range<range_type>;
        requires rfwd_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
        requires rmut_fwd_iter_pair<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_fwd_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_fwd_range_of = requires
    {
        requires _rmut_range_of<range_type, type>;
        requires rfwd_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
        requires rmut_fwd_iter_pair_of<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rbidi_range = requires
    {
        requires _rrange<range_type>;
        requires rbidi_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `bidi_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rbidi_range_of = requires
    {
        requires _rrange_of<range_type, type>;
        requires rbidi_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_bidi_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_bidi_range = requires
    {
        requires _rmut_range<range_type>;
        requires rbidi_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
        requires rmut_bidi_iter_pair<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_bidi_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_bidi_range_of = requires
    {
        requires _rmut_range_of<range_type, type>;
        requires rbidi_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
        requires rmut_bidi_iter_pair_of<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rjump_range = requires
    {
        requires _rrange<range_type>;
        requires rjump_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `jump_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rjump_range_of = requires
    {
        requires _rrange_of<range_type, type>;
        requires rjump_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_jump_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_jump_range = requires
    {
        requires _rmut_range<range_type>;
        requires rjump_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
        requires rmut_jump_iter_pair<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_jump_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_jump_range_of = requires
    {
        requires _rmut_range_of<range_type, type>;
        requires rjump_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
        requires rmut_jump_iter_pair_of<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rarray_range = requires
    {
        requires _rrange<range_type>;
        requires rarray_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `array_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rarray_range_of = requires
    {
        requires _rrange_of<range_type, type>;
        requires rarray_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_array_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_array_range = requires
    {
        requires _rmut_range<range_type>;
        requires rarray_iter_pair<typename range_type::iter_type, typename range_type::iter_end_type>;
        requires rmut_array_iter_pair<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_array_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_array_range_of = requires
    {
        requires _rmut_range_of<range_type, type>;
        requires rarray_iter_pair_of<typename range_type::iter_type, typename range_type::iter_end_type, type>;
        requires rmut_array_iter_pair_of<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename>
    concept rrev_range = requires
    {
        requires rbidi_range<range_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `rev_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rrev_range_of = requires
    {
        requires rbidi_range_of<range_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_rev_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_rev_range = requires
    {
        requires rmut_bidi_range<range_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_rev_range` of type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename type>
    concept rmut_rev_range_of = requires
    {
        requires rmut_bidi_range_of<range_type, type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rcommon_range = requires
    {
        requires _rrange<range_type>;
        requires rsame_as<typename range_type::iter_type, typename range_type::iter_end_type>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `range_type` is `mut_common_range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    concept rmut_common_range = requires
    {
        requires rcommon_range<range_type>;
        requires rsame_as<typename range_type::mut_iter_type, typename range_type::mut_iter_end_type>;
    };
}
