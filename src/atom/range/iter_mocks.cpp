export module atom.core:range.iter_mocks;
import :core;
import :mem_ptr;

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// mock object used as `iter_end`.
    /// --------------------------------------------------------------------------------------------
    class iter_end
    {};

    /// --------------------------------------------------------------------------------------------
    /// `riter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class iter
    {
    public:
        using elem_type = in_value_type;

    public:
        iter() = delete;
        iter(const iter& that) = delete;
        iter& operator=(const iter& that) = delete;

        iter(iter&& that) = default;
        iter& operator=(iter&& that) = default;
        ~iter() = default;

    public:
        auto value() const -> const elem_type&;
        auto is_eq(iter_end end) const -> bool;
        auto next() -> iter&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        mut_iter() = delete;
        mut_iter(const mut_iter& that) = delete;
        mut_iter& operator=(const mut_iter& that) = delete;

        mut_iter(mut_iter&& that) = default;
        mut_iter& operator=(mut_iter&& that) = default;
        ~mut_iter() = default;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto is_eq(iter_end end) const -> bool;
        auto next() -> mut_iter&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rfwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class fwd_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto next() -> fwd_iter&;
        auto is_eq(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_fwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_fwd_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto next() -> mut_fwd_iter&;
        auto is_eq(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rbidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class bidi_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto next() -> bidi_iter&;
        auto prev() -> bidi_iter&;
        auto is_eq(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_bidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_bidi_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto next() -> mut_bidi_iter&;
        auto prev() -> mut_bidi_iter&;
        auto is_eq(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rjump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class jump_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto next(usize steps = 1) -> jump_iter&;
        auto prev(usize steps = 1) -> jump_iter&;
        auto is_eq(iter_end end) const -> bool;
        auto compare(const jump_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_jump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_jump_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto next(usize steps = 1) -> mut_jump_iter&;
        auto prev(usize steps = 1) -> mut_jump_iter&;
        auto is_eq(iter_end end) const -> bool;
        auto compare(const mut_jump_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rarray_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class array_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto data() const -> mem_ptr<in_value_type>;
        auto next(usize steps = 1) -> array_iter&;
        auto prev(usize steps = 1) -> array_iter&;
        auto is_eq(array_iter end) const -> bool;
        auto compare(const array_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `rmut_array_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_array_iter
    {
    public:
        using elem_type = in_value_type;

    public:
        auto value() const -> const elem_type&;
        auto mut_value() -> elem_type&;
        auto data() const -> mem_ptr<in_value_type>;
        auto mut_data() -> mut_mem_ptr<in_value_type>;
        auto next(usize steps = 1) -> mut_array_iter&;
        auto prev(usize steps = 1) -> mut_array_iter&;
        auto is_eq(mut_array_iter end) const -> bool;
        auto compare(const mut_array_iter& that) const -> isize;
    };
}
