export module atom_core:ranges.range_definition;

import std;
import :core;

export namespace atom::ranges
{
    template <typename range_type>
    class range_definition;

    template <typename in_value_type, usize count>
    class range_definition<in_value_type[count]>
    {
    public:
        using value_type = in_value_type;
        using const_iterator_type = const value_type*;
        using const_iterator_end_type = const value_type*;
        using iterator_type = value_type*;
        using iterator_end_type = value_type*;

    public:
        static constexpr auto get_iterator(value_type (&arr)[count]) -> value_type*
        {
            return arr;
        }

        static constexpr auto get_iterator_end(value_type (&arr)[count]) -> value_type*
        {
            return arr + count;
        }

        static constexpr auto get_const_iterator(
            const value_type (&arr)[count]) -> const value_type*
        {
            return arr;
        }

        static constexpr auto get_const_iterator_end(
            const value_type (&arr)[count]) -> const value_type*
        {
            return arr + count;
        }
    };

    template <typename in_value_type, usize count>
    class range_definition<const in_value_type[count]>
    {
    public:
        using value_type = in_value_type;
        using const_iterator_type = const value_type*;
        using const_iterator_end_type = const value_type*;

    public:
        static constexpr auto get_const_iterator(
            const value_type (&arr)[count]) -> const value_type*
        {
            return arr;
        }

        static constexpr auto get_const_iterator_end(
            const value_type (&arr)[count]) -> const value_type*
        {
            return arr + count;
        }
    };

    // template <typename std_range_type>
    //     requires(std::ranges::range<std_range_type>)
    // class range_definition<std_range_type>
    // {
    // public:
    //     using value_type = std::ranges::value_type<std_range_type>;
    //     using const_iterator_type = std::ranges::const_iterator_t<std_range_type>;
    //     using const_iterator_end_type = std::ranges::const_sentinel_t<std_range_type>;
    //     using iterator_type = std::ranges::iterator_t<std_range_type>;
    //     using iterator_end_type = std::ranges::sentinel_t<std_range_type>;

    //     constexpr auto get_iterator(std_range_type& range) -> value_type*
    //     {
    //         return std::ranges::begin(range);
    //     }

    //     constexpr auto get_iterator_end(std_range_type& range) -> value_type*
    //     {
    //         return std::ranges::end(range);
    //     }

    //     constexpr auto get_const_iterator(const std_range_type& range) -> const value_type*
    //     {
    //         return std::ranges::begin(range);
    //     }

    //     constexpr auto get_const_iterator_end(const std_range_type& range) -> const value_type*
    //     {
    //         return std::ranges::end(range);
    //     }
    // };
}
