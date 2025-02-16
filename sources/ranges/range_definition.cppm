export module atom_core:ranges.range_definition;

import std;
import :core;

namespace atom::ranges
{
    export template <typename range_type>
    class range_definition;

    export template <typename in_value_type, usize count>
    class range_definition<const in_value_type[count]>
    {
    public:
        using value_type = in_value_type;
        using const_iterator_type = const value_type*;
        using const_iterator_end_type = const value_type*;

    public:
        static constexpr auto get_const_iterator(const value_type* arr) -> const value_type*
        {
            return arr;
        }

        static constexpr auto get_const_iterator_end(const value_type* arr) -> const value_type*
        {
            return arr + count;
        }
    };

    export template <typename in_value_type, usize count>
    class range_definition<in_value_type[count]>
        : public range_definition<const in_value_type[count]>
    {
    public:
        using value_type = in_value_type;
        using iterator_type = value_type*;
        using iterator_end_type = value_type*;

    public:
        static constexpr auto get_iterator(value_type* arr) -> value_type*
        {
            return arr;
        }

        static constexpr auto get_iterator_end(value_type* arr) -> value_type*
        {
            return arr + count;
        }
    };

    template <typename std_range_type>
    struct _is_std_range
    {
        static constexpr bool value = false;
    };

    template <typename value_type>
    struct _is_std_range<std::initializer_list<value_type>>
    {
        static constexpr bool value = true;
    };

    template <typename value_type>
    struct _is_std_range<std::basic_string<value_type>>
    {
        static constexpr bool value = true;
    };

    template <typename value_type>
    struct _is_std_range<std::basic_string_view<value_type>>
    {
        static constexpr bool value = true;
    };

    template <typename value_type>
    struct _is_std_range<std::vector<value_type>>
    {
        static constexpr bool value = true;
    };

    template <typename value_type, std::size_t count>
    struct _is_std_range<std::array<value_type, count>>
    {
        static constexpr bool value = true;
    };

    export template <typename std_range_type>
        requires(_is_std_range<std_range_type>::value)
    class range_definition<std_range_type>
    {
    public:
        using value_type = std::ranges::range_value_t<std_range_type>;
        using iterator_type = std::ranges::iterator_t<std_range_type>;
        using iterator_end_type = std::ranges::sentinel_t<std_range_type>;
        using const_iterator_type = decltype(std::ranges::cbegin(std::declval<std_range_type&>()));
        using const_iterator_end_type =
            decltype(std::ranges::cend(std::declval<std_range_type&>()));

    public:
        static constexpr auto get_const_iterator(const std_range_type& range) -> const_iterator_type
        {
            return std::ranges::cbegin(range);
        }

        static constexpr auto get_const_iterator_end(const std_range_type& range)
            -> const_iterator_end_type
        {
            return std::ranges::cend(range);
        }

        static constexpr auto get_iterator(std_range_type& range) -> iterator_type
        {
            return std::ranges::begin(range);
        }

        static constexpr auto get_iterator_end(std_range_type& range) -> iterator_end_type
        {
            return std::ranges::end(range);
        }
    };
}
