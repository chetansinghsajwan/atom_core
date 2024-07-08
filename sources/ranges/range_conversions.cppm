export module atom_core:ranges.range_conversions;

import std;
import :core;
import :ranges.iterator_definition;
import :ranges.iterator_concepts;
import :ranges.range_definition;
import :ranges.range_concepts;

namespace atom::ranges
{
    template <typename iterator_type, typename iterator_end_type>
    class _range_from_iterator_pair
    {
    public:
        constexpr _range_from_iterator_pair(iterator_type it, iterator_end_type it_end)
            : it{ move(it) }
            , it_end{ move(it_end) }
        {}

    public:
        iterator_type it;
        iterator_end_type it_end;
    };

    template <typename in_iterator_type, typename in_iterator_end_type>
    class range_definition<_range_from_iterator_pair<in_iterator_type, in_iterator_end_type>>
    {
        using range_type = _range_from_iterator_pair<in_iterator_type, in_iterator_end_type>;

        static constexpr bool is_constant_range =
            not iterator_pair_concept<in_iterator_type, in_iterator_end_type>;

    public:
        static_assert(const_iterator_pair_concept<in_iterator_type, in_iterator_end_type>);

    public:
        using value_type = typename iterator_definition<in_iterator_type>::value_type;
        using iterator_type = in_iterator_type;
        using iterator_end_type = in_iterator_end_type;
        using const_iterator_type = in_iterator_type;
        using const_iterator_end_type = in_iterator_end_type;

    public:
        static constexpr auto get_iterator(range_type& range) -> iterator_type
            requires(is_constant_range)
        {
            return range.it;
        }

        static constexpr auto get_iterator_end(range_type& range) -> iterator_type
            requires(is_constant_range)
        {
            return range.it_end;
        }

        static constexpr auto get_const_iterator(const range_type& range) -> const_iterator_type
        {
            return range.it;
        }

        static constexpr auto get_const_iterator_end(const range_type& range) -> const_iterator_type
        {
            return range.it_end;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// # to do
    ///
    /// - review this implementation after implementing character encoding.
    /// --------------------------------------------------------------------------------------------
    constexpr auto _find_str_len(const char* str) -> usize
    {
        if (std::is_constant_evaluated())
        {
            usize len = 0;
            while (*str != '\0')
            {
                str++;
                len++;
            }

            return len + 1;
        }

        return std::strlen(str);
    }
}

export namespace atom::ranges
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, const value_type* end)
    {
        return _range_from_iterator_pair{ begin, end };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, value_type* end)
    {
        return _range_from_iterator_pair{ begin, end };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(const value_type* begin, usize count)
    {
        return _range_from_iterator_pair{ begin, begin + count };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    constexpr auto from(value_type* begin, usize count)
    {
        return _range_from_iterator_pair{ begin, begin + count };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(const char* str)
    {
        return _range_from_iterator_pair{ str, str + _find_str_len(str) + 1 };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    constexpr auto from(char* str)
    {
        return _range_from_iterator_pair{ str, str + _find_str_len(str) + 1 };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename iterator_type, typename iterator_end_type>
    constexpr auto from(iterator_type it, iterator_end_type it_end)
        requires(const_iterator_pair_concept<iterator_type, iterator_end_type>)
    {
        return _range_from_iterator_pair{ move(it), move(it_end) };
    }
}
