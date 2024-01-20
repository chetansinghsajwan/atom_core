export module atom.core:range.iter_requirements;
import :requirements;
import :core;
import :mem_ptr;

// clang-format off
namespace atom
{
    template <typename iter_type, typename type>
    concept _riter_of = requires
    {
        requires rconvertible_to<typename iter_type::elem_type, type>;
    };

    template <typename iter_type>
    concept _rmut_iter = requires(iter_type it)
    {
        { it.mut_value() } -> rconvertible_to<typename iter_type::elem_type&>;
    };

    template <typename iter_type, typename type>
    concept _rmut_iter_of = requires
    {
        requires rsame_as<typename iter_type::elem_type, type>;
        requires _rmut_iter<iter_type>;
    };
}

export namespace atom
{
    template <typename iter_type, typename iter_end_type>
    concept riter_with_end = requires(iter_type it, iter_end_type it_end)
    {
        requires rcopyable<iter_end_type>;
        requires rmoveable<iter_end_type>;
        requires rdestructible<iter_end_type>;

        { it.eq(it_end) } -> rsame_as<bool>;
    };

    template <typename iter_type>
    concept riter = requires(iter_type it, const iter_type cit)
    {
        typename iter_type::elem_type;

        requires rmoveable<iter_type>;
        requires rdestructible<iter_type>;

        { cit.value() } -> rconvertible_to<const typename iter_type::elem_type&>;
        { it.next() } -> rconvertible_to<iter_type&>;
    };

    template <typename iter_type>
    concept rmut_iter = requires
    {
        requires riter<iter_type>;
        requires _rmut_iter<iter_type>;
    };

    template <typename iter_type, typename type>
    concept riter_of = requires
    {
        requires riter<iter_type>;
        requires _riter_of<iter_type, type>;
    };

    template <typename iter_type, typename type>
    concept rmut_iter_of = requires
    {
        requires rmut_iter<iter_type>;
        requires _rmut_iter_of<iter_type, type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept riter_pair = requires
    {
        requires riter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rmut_iter_pair = requires
    {
        requires rmut_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept riter_pair_of = requires
    {
        requires riter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rmut_iter_pair_of = requires
    {
        requires rmut_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type>
    concept rfwd_iter = requires
    {
        requires riter<iter_type>;
        requires rcopyable<iter_type>;
    };

    template <typename iter_type>
    concept rmut_fwd_iter = requires
    {
        requires rfwd_iter<iter_type>;
        requires _rmut_iter<iter_type>;
    };

    template <typename iter_type, typename type>
    concept rfwd_iter_of = requires
    {
        requires rfwd_iter<iter_type>;
        requires _riter_of<iter_type, type>;
    };

    template <typename iter_type, typename type>
    concept rmut_fwd_iter_of = requires
    {
        requires rmut_fwd_iter<iter_type>;
        requires _rmut_iter_of<iter_type, type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rfwd_iter_pair = requires
    {
        requires rfwd_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rmut_fwd_iter_pair = requires
    {
        requires rmut_fwd_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rfwd_iter_pair_of = requires
    {
        requires rfwd_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rmut_fwd_iter_pair_of = requires
    {
        requires rmut_fwd_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type>
    concept rbidi_iter = requires(iter_type it)
    {
        requires rfwd_iter<iter_type>;

        { it.prev() } -> rconvertible_to<iter_type&>;
    };

    template <typename iter_type>
    concept rmut_bidi_iter = requires
    {
        requires rbidi_iter<iter_type>;
        requires _rmut_iter<iter_type>;
    };

    template <typename iter_type, typename type>
    concept rbidi_iter_of = requires
    {
        requires rbidi_iter<iter_type>;
        requires _riter_of<iter_type, type>;
    };

    template <typename iter_type, typename type>
    concept rmut_bidi_iter_of = requires
    {
        requires rmut_bidi_iter<iter_type>;
        requires _rmut_iter_of<iter_type, type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rbidi_iter_pair = requires
    {
        requires rbidi_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rmut_bidi_iter_pair = requires
    {
        requires rmut_bidi_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rbidi_iter_pair_of = requires
    {
        requires rbidi_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rmut_bidi_iter_pair_of = requires
    {
        requires rmut_bidi_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type>
    concept rjump_iter = requires(iter_type it, const iter_type cit, usize steps)
    {
        requires rbidi_iter<iter_type>;

        { it.next(steps) } -> rconvertible_to<iter_type&>;
        { it.prev(steps) } -> rconvertible_to<iter_type&>;
        { cit.compare(cit) } -> rconvertible_to<isize>;
    };

    template <typename iter_type>
    concept rmut_jump_iter = requires
    {
        requires rjump_iter<iter_type>;
        requires _rmut_iter<iter_type>;
    };

    template <typename iter_type, typename type>
    concept rjump_iter_of = requires
    {
        requires rjump_iter<iter_type>;
        requires _riter_of<iter_type, type>;
    };

    template <typename iter_type, typename type>
    concept rmut_jump_iter_of = requires
    {
        requires rmut_jump_iter<iter_type>;
        requires _rmut_iter_of<iter_type, type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rjump_iter_pair = requires
    {
        requires rjump_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rmut_jump_iter_pair = requires
    {
        requires rmut_jump_iter<iter_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rjump_iter_pair_of = requires
    {
        requires rjump_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rmut_jump_iter_pair_of = requires
    {
        requires rmut_jump_iter_of<iter_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type>
    concept rarray_iter = requires(const iter_type cit)
    {
        requires rjump_iter<iter_type>;

        { cit.data() } -> rconvertible_to<mem_ptr<typename iter_type::elem_type>>;
    };

    template <typename iter_type>
    concept rmut_array_iter = requires(iter_type it)
    {
        requires rarray_iter<iter_type>;
        requires _rmut_iter<iter_type>;

        { it.mut_data() } -> rconvertible_to<mut_mem_ptr<typename iter_type::elem_type>>;
    };

    template <typename iter_type, typename type>
    concept rarray_iter_of = requires
    {
        requires rarray_iter<iter_type>;
        requires _riter_of<iter_type, type>;
    };

    template <typename iter_type, typename type>
    concept rmut_array_iter_of = requires
    {
        requires rmut_array_iter<iter_type>;
        requires _rmut_iter_of<iter_type, type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rarray_iter_pair = requires
    {
        requires rarray_iter<iter_type>;
        requires rarray_iter<iter_end_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type>
    concept rmut_array_iter_pair = requires
    {
        requires rmut_array_iter<iter_type>;
        requires rmut_array_iter<iter_end_type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rarray_iter_pair_of = requires
    {
        requires rarray_iter_of<iter_type, type>;
        requires rarray_iter_of<iter_end_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };

    template <typename iter_type, typename iter_end_type, typename type>
    concept rmut_array_iter_pair_of = requires
    {
        requires rmut_array_iter_of<iter_type, type>;
        requires rmut_array_iter_of<iter_end_type, type>;
        requires riter_with_end<iter_type, iter_end_type>;
    };
}

// clang-format on
