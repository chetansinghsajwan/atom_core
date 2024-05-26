export module atom.core:ranges.iter_mocks;

import :core;

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// mock object used as `iter_end`.
    /// --------------------------------------------------------------------------------------------
    class iter_end
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class iter
    {
    public:
        using value_t = in_value_t;

    public:
        iter() = delete;
        iter(const iter& that) = delete;
        iter& operator=(const iter& that) = delete;

        iter(iter&& that) = default;
        iter& operator=(iter&& that) = default;
        ~iter() = default;

    public:
        auto value() const -> const value_t&;
        auto operator==(iter_end end) const -> bool;
        auto next() -> iter&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_iter
    {
    public:
        using value_t = in_value_t;

    public:
        mut_iter() = delete;
        mut_iter(const mut_iter& that) = delete;
        mut_iter& operator=(const mut_iter& that) = delete;

        mut_iter(mut_iter&& that) = default;
        mut_iter& operator=(mut_iter&& that) = default;
        ~mut_iter() = default;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto operator==(iter_end end) const -> bool;
        auto next() -> mut_iter&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_fwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class fwd_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> fwd_iter&;
        auto operator==(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_fwd_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_fwd_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> mut_fwd_iter&;
        auto operator==(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_bidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class bidi_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto next() -> bidi_iter&;
        auto prev() -> bidi_iter&;
        auto operator==(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidi_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_bidi_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> mut_bidi_iter&;
        auto prev() -> mut_bidi_iter&;
        auto operator==(iter_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_jump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class jump_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto next(usize steps = 1) -> jump_iter&;
        auto prev(usize steps = 1) -> jump_iter&;
        auto operator==(iter_end end) const -> bool;
        auto compare(const jump_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_jump_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_jump_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next(usize steps = 1) -> mut_jump_iter&;
        auto prev(usize steps = 1) -> mut_jump_iter&;
        auto operator==(iter_end end) const -> bool;
        auto compare(const mut_jump_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_array_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class array_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto get_data() const -> const in_value_t*;
        auto next(usize steps = 1) -> array_iter&;
        auto prev(usize steps = 1) -> array_iter&;
        auto operator==(array_iter end) const -> bool;
        auto compare(const array_iter& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_iter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_array_iter
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto get_data() const -> const in_value_t*;
        auto get_mut_data() -> in_value_t*;
        auto next(usize steps = 1) -> mut_array_iter&;
        auto prev(usize steps = 1) -> mut_array_iter&;
        auto operator==(mut_array_iter end) const -> bool;
        auto compare(const mut_array_iter& that) const -> isize;
    };
}
