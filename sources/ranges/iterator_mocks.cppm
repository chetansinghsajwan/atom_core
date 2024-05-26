export module atom.core:ranges.iterator_mocks;

import :core;

export namespace atom::mock
{
    /// --------------------------------------------------------------------------------------------
    /// mock object used as `iterator_end`.
    /// --------------------------------------------------------------------------------------------
    class iterator_end
    {};

    /// --------------------------------------------------------------------------------------------
    /// `is_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class iterator
    {
    public:
        using value_t = in_value_t;

    public:
        iterator() = delete;
        iterator(const iterator& that) = delete;
        iterator& operator=(const iterator& that) = delete;

        iterator(iterator&& that) = default;
        iterator& operator=(iterator&& that) = default;
        ~iterator() = default;

    public:
        auto value() const -> const value_t&;
        auto operator==(iterator_end end) const -> bool;
        auto next() -> iterator&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        mut_iterator() = delete;
        mut_iterator(const mut_iterator& that) = delete;
        mut_iterator& operator=(const mut_iterator& that) = delete;

        mut_iterator(mut_iterator&& that) = default;
        mut_iterator& operator=(mut_iterator&& that) = default;
        ~mut_iterator() = default;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto operator==(iterator_end end) const -> bool;
        auto next() -> mut_iterator&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_unidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class unidirectional_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> unidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_unidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_unidirectional_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> mut_unidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_bidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class bidirectional_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto next() -> bidirectional_iterator&;
        auto prev() -> bidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_bidirectional_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next() -> mut_bidirectional_iterator&;
        auto prev() -> mut_bidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_random_access_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class random_access_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto next(usize steps = 1) -> random_access_iterator&;
        auto prev(usize steps = 1) -> random_access_iterator&;
        auto operator==(iterator_end end) const -> bool;
        auto compare(const random_access_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_random_access_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_random_access_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto next(usize steps = 1) -> mut_random_access_iterator&;
        auto prev(usize steps = 1) -> mut_random_access_iterator&;
        auto operator==(iterator_end end) const -> bool;
        auto compare(const mut_random_access_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_array_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class array_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto get_data() const -> const in_value_t*;
        auto next(usize steps = 1) -> array_iterator&;
        auto prev(usize steps = 1) -> array_iterator&;
        auto operator==(array_iterator end) const -> bool;
        auto compare(const array_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class mut_array_iterator
    {
    public:
        using value_t = in_value_t;

    public:
        auto value() const -> const value_t&;
        auto mut_value() -> value_t&;
        auto get_data() const -> const in_value_t*;
        auto get_mut_data() -> in_value_t*;
        auto next(usize steps = 1) -> mut_array_iterator&;
        auto prev(usize steps = 1) -> mut_array_iterator&;
        auto operator==(mut_array_iterator end) const -> bool;
        auto compare(const mut_array_iterator& that) const -> isize;
    };
}
