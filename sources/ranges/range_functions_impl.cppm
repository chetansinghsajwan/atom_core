export module atom_core:ranges.range_functions_impl;

import std;
import :core;
import :types;
import :ranges.iterator_concepts;
import :ranges.range_concepts;
import :ranges.range_definition;

namespace atom::ranges
{
    template <typename range_type>
    class range_functions_impl
    {
    private:
        template <typename other_range_type>
        struct _alias_helper
        {
            using iterator_type = type_utils::empty_type;
            using iterator_end_type = type_utils::empty_type;
        };

        template <typename other_range_type>
            requires(ranges::range_concept<other_range_type>)
        struct _alias_helper<other_range_type>
        {
            using iterator_type = typename range_definition<range_type>::iterator_type;
            using iterator_end_type = typename range_definition<range_type>::iterator_end_type;
        };

    public:
        using value_type = typename range_definition<range_type>::value_type;
        using const_iterator_type = typename range_definition<range_type>::const_iterator_type;
        using const_iterator_end_type =
            typename range_definition<range_type>::const_iterator_end_type;
        using iterator_type = typename _alias_helper<range_type>::iterator_type;
        using iterator_end_type = typename _alias_helper<range_type>::iterator_end_type;

    public:
        static constexpr auto get_iterator(const range_type& range) -> const_iterator_type
        {
            return range_definition<range_type>::get_const_iterator(range);
        }

        static constexpr auto get_iterator_end(const range_type& range) -> const_iterator_end_type
        {
            return range_definition<range_type>::get_const_iterator_end(range);
        }

        static constexpr auto get_iterator_at(
            const range_type& range, usize i) -> const_iterator_type
        {
            return range_definition<range_type>::get_const_iterator(range) + i;
        }

        static constexpr auto get_iterator(range_type& range) -> iterator_type
            requires ranges::range_concept<range_type>
        {
            return range_definition<range_type>::get_iterator(range);
        }

        static constexpr auto get_iterator_end(range_type& range) -> iterator_end_type
            requires ranges::range_concept<range_type>
        {
            return range_definition<range_type>::get_iterator_end(range);
        }

        static constexpr auto get_iterator_at(range_type& range, usize i) -> iterator_type
            requires ranges::range_concept<range_type>
        {
            return range_definition<range_type>::get_iterator(range) + i;
        }

        static constexpr auto get_data(const range_type& range) -> const value_type*
        {
            return &*get_iterator(range);
        }

        static constexpr auto get_data(range_type& range) -> value_type*
        {
            return &*get_iterator(range);
        }

        static constexpr auto get_at(const range_type& range, usize i) -> const value_type&
        {
            return get_data(range)[i];
        }

        static constexpr auto get_at(range_type& range, usize i) -> value_type&
        {
            return get_data(range)[i];
        }

        static constexpr auto get_first(const range_type& range) -> const value_type&
        {
            return get_data(range)[0];
        }

        static constexpr auto get_first(range_type& range) -> value_type&
        {
            return get_data(range)[0];
        }

        static constexpr auto get_last(const range_type& range) -> const value_type&
        {
            return get_data(range)[get_count(range) - 1];
        }

        static constexpr auto get_last(range_type& range) -> value_type&
        {
            return get_data(range)[get_count(range) - 1];
        }

        static constexpr auto get_count(const range_type& range) -> usize
        {
            return get_iterator_end(range) - get_iterator(range);
        }

        static constexpr auto is_empty(const range_type& range) -> bool
        {
            return get_count(range) == 0;
        }

        template <typename that_value_type>
        static constexpr auto find_value(
            const range_type& range, const that_value_type& value) -> const_iterator_type
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);

            return std::find(begin, end, value);
        }

        template <typename function_type>
        static constexpr auto find_if(
            const range_type& range, const function_type& pred) -> const_iterator_type
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);

            return std::find_if(begin, end, pred);
        }

        template <typename that_range_type>
        static constexpr auto find_range(
            const range_type& range, const that_range_type& that_range) -> const_iterator_type
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);
            auto that_begin = get_iterator(that_range);
            auto that_end = get_iterator_end(that_range);

            return std::search(begin, end, that_begin, that_end);
        }

        template <typename that_range_type>
        static constexpr auto compare(
            const range_type& range, const that_range_type& that_range) -> i8
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);
            auto that_begin = get_iterator(that_range);
            auto that_end = get_iterator_end(that_range);

            return not std::equal(begin, end, that_begin, that_end);
        }

        template <typename that_range_type>
        static constexpr auto is_eq(
            const range_type& range, const that_range_type& that_range) -> bool
        {
            return compare(range, that_range) == 0;
        }

        static constexpr auto count_values(const range_type& range) -> usize
        {
            if constexpr (ranges::const_random_access_iterator_pair_concept<const_iterator_type,
                              const_iterator_end_type>)
            {
                return get_iterator_end(range) - get_iterator(range);
            }

            usize count = 0;
            for (auto it = get_iterator(range); it != get_iterator_end(range); it++)
                count++;

            return count;
        }

        static constexpr auto is_index_in_range(const range_type& range, usize i) -> bool
        {
            return i < get_count(range);
        }

        template <typename that_value_type>
        static constexpr auto contains(
            const range_type& range, const that_value_type& value) -> bool
        {
            return std::ranges::contains(get_iterator(range), get_iterator_end(range), value);
        }

        template <typename function_type>
        static constexpr auto contains_if(
            const range_type& range, const function_type& pred) -> bool
        {
            return std::find_if(get_iterator(range), get_iterator_end(range), pred)
                   != get_iterator_end(range);
        }

        template <typename that_range_type>
        static constexpr auto contains_range(
            const range_type& range, const that_range_type& that_range) -> bool
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);
            auto that_begin = get_iterator(that_range);
            auto that_end = get_iterator_end(that_range);

            return std::search(begin, end, that_begin, that_end) != end;
        }
    };
}
