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
    template <typename in_value_type>
    class iterator
    {
    public:
        using value_type = in_value_type;

    public:
        iterator() = delete;
        iterator(const iterator& that) = delete;
        iterator& operator=(const iterator& that) = delete;

        iterator(iterator&& that) = default;
        iterator& operator=(iterator&& that) = default;
        ~iterator() = default;

    public:
        auto value() const -> const value_type&;
        auto operator==(iterator_end end) const -> bool;
        auto next() -> iterator&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        mut_iterator() = delete;
        mut_iterator(const mut_iterator& that) = delete;
        mut_iterator& operator=(const mut_iterator& that) = delete;

        mut_iterator(mut_iterator&& that) = default;
        mut_iterator& operator=(mut_iterator&& that) = default;
        ~mut_iterator() = default;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto operator==(iterator_end end) const -> bool;
        auto next() -> mut_iterator&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_unidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class unidirectional_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto next() -> unidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_unidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_unidirectional_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto next() -> mut_unidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_bidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class bidirectional_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto next() -> bidirectional_iterator&;
        auto prev() -> bidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_bidirectional_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_bidirectional_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto next() -> mut_bidirectional_iterator&;
        auto prev() -> mut_bidirectional_iterator&;
        auto operator==(iterator_end end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_random_access_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class random_access_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto next(usize steps = 1) -> random_access_iterator&;
        auto prev(usize steps = 1) -> random_access_iterator&;
        auto operator==(iterator_end end) const -> bool;
        auto compare(const random_access_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_random_access_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_random_access_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto next(usize steps = 1) -> mut_random_access_iterator&;
        auto prev(usize steps = 1) -> mut_random_access_iterator&;
        auto operator==(iterator_end end) const -> bool;
        auto compare(const mut_random_access_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_array_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class array_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto get_data() const -> const in_value_type*;
        auto next(usize steps = 1) -> array_iterator&;
        auto prev(usize steps = 1) -> array_iterator&;
        auto operator==(array_iterator end) const -> bool;
        auto compare(const array_iterator& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `is_mut_array_iterator` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class mut_array_iterator
    {
    public:
        using value_type = in_value_type;

    public:
        auto value() const -> const value_type&;
        auto mut_value() -> value_type&;
        auto get_data() const -> const in_value_type*;
        auto get_mut_data() -> in_value_type*;
        auto next(usize steps = 1) -> mut_array_iterator&;
        auto prev(usize steps = 1) -> mut_array_iterator&;
        auto operator==(mut_array_iterator end) const -> bool;
        auto compare(const mut_array_iterator& that) const -> isize;
    };
}
