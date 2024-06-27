export module atom.core:ranges.range_extensions_impl;

import std;
import :core;
import :types;
import :ranges.iterator_requirements;
import :ranges.range_requirements;

namespace atom
{
    template <typename range_type>
    class _range_extensions_impl
    {
    protected:
        using _impl_type = range_type;

    private:
        template <typename in_range_type>
        struct _mut_aliases_resolver
        {
            using mut_iterator_type = type_utils::empty_type;
            using mut_iterator_end_type = type_utils::empty_type;
        };

        template <typename in_range_type>
            requires ranges::is_mut_range<in_range_type>
        struct _mut_aliases_resolver<in_range_type>
        {
            using mut_iterator_type = typename in_range_type::mut_iterator_type;
            using mut_iterator_end_type = typename in_range_type::mut_iterator_end_type;
        };

    public:
        using value_type = typename _impl_type::value_type;
        using iterator_type = typename _impl_type::iterator_type;
        using iterator_end_type = typename _impl_type::iterator_end_type;
        using mut_iterator_type = typename _mut_aliases_resolver<_impl_type>::mut_iterator_type;
        using mut_iterator_end_type =
            typename _mut_aliases_resolver<_impl_type>::mut_iterator_end_type;

    public:
        static constexpr auto get_iterator(const range_type& range) -> iterator_type
        {
            return range.get_iterator();
        }

        static constexpr auto get_iterator_end(const range_type& range) -> iterator_end_type
        {
            return range.get_iterator_end();
        }

        static constexpr auto get_iterator_at(const range_type& range, usize i) -> iterator_type
        {
            return range.get_iterator().next(i);
        }

        static constexpr auto get_mut_iterator(range_type& range) -> mut_iterator_type
            requires ranges::is_mut_range<range_type>
        {
            return range.get_mut_iterator();
        }

        static constexpr auto get_mut_iterator_end(range_type& range) -> mut_iterator_end_type
            requires ranges::is_mut_range<range_type>
        {
            return range.get_mut_iterator_end();
        }

        static constexpr auto get_mut_iterator_at(range_type& range, usize i) -> iterator_type
            requires ranges::is_mut_range<range_type>
        {
            return range.get_mut_iterator().next(i);
        }

        // static constexpr auto begin(const range_type& range) -> std_iterator_type
        // {
        //     return get_iterator(range);
        // }

        // static constexpr auto end(const range_type& range) -> std_iterator_end_type
        // {
        //     return get_iterator_end(range);
        // }

        static constexpr auto begin(range_type& range)
        // -> std_mut_iterator_type
        {
            if constexpr (ranges::is_mut_range<range_type>)
                return get_mut_iterator(range);
            else
                return get_iterator(range);
        }

        static constexpr auto end(range_type& range)
        // -> std_mut_iterator_end_type
        {
            if constexpr (ranges::is_mut_range<range_type>)
                return get_mut_iterator_end(range);
            else
                return get_iterator_end(range);
        }

        static constexpr auto get_data(const range_type& range) -> const value_type*
        {
            return range.get_data();
        }

        static constexpr auto get_mut_data(range_type& range) -> value_type*
        {
            return range.get_mut_data();
        }

        static constexpr auto get_at(const range_type& range, usize i) -> const value_type&
        {
            return get_data(range)[i];
        }

        static constexpr auto get_mut_at(range_type& range, usize i) -> value_type&
        {
            return get_mut_data(range)[i];
        }

        static constexpr auto get_front(const range_type& range) -> const value_type&
        {
            return get_data(range)[0];
        }

        static constexpr auto get_front_mut(range_type& range) -> value_type&
        {
            return get_mut_data(range)[0];
        }

        static constexpr auto get_back(const range_type& range) -> const value_type&
        {
            return get_data(range)[get_count(range) - 1];
        }

        static constexpr auto get_back_mut(range_type& range) -> value_type&
        {
            return get_mut_data(range)[get_count(range) - 1];
        }

        static constexpr auto get_count(const range_type& range) -> usize
        {
            return range.get_count();
        }

        static constexpr auto is_empty(const range_type& range) -> bool
        {
            return get_count(range) == 0;
        }

        template <typename that_value_type>
        static constexpr auto find_elem(
            const range_type& range, const that_value_type& value) -> iterator_type
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);

            return std::find(begin, end, value);
        }

        template <typename function_type>
        static constexpr auto find_if(
            const range_type& range, const function_type& pred) -> iterator_type
        {
            auto begin = get_iterator(range);
            auto end = get_iterator_end(range);

            return std::find_if(begin, end, pred);
        }

        template <typename that_range_type>
        static constexpr auto find_range(
            const range_type& range, const that_range_type& that_range) -> iterator_type
        {
            auto this_begin = get_iterator(range);
            auto this_end = get_iterator_end(range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return std::search(this_begin, this_end, that_begin, that_end);
        }

        template <typename that_range_type>
        static constexpr auto compare(const range_type& range, that_range_type& that_range) -> i8
        {
            auto this_begin = get_iterator(range);
            auto this_end = get_iterator_end(range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return not std::equal(this_begin, this_end, that_begin, that_end);
        }

        template <typename that_range_type>
        static constexpr auto is_eq(
            const range_type& range, const that_range_type& that_range) -> bool
        {
            return compare(range, that_range) == 0;
        }

        static constexpr auto count_elems(const range_type& range) -> usize
        {
            if constexpr (ranges::is_random_access_iterator_pair<iterator_type, iterator_end_type>)
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
            return std::ranges::contains(range.get_iterator(), range.get_iterator_end(), value);
        }

        template <typename function_type>
        static constexpr auto contains_if(
            const range_type& range, const function_type& pred) -> bool
        {
            return std::find_if(range.get_iterator(), range.get_iterator_end(), pred)
                   != range.get_iterator_end();
        }

        template <typename that_range_type>
        static constexpr auto contains_range(
            const range_type& range, const that_range_type& that_range) -> bool
        {
            return std::search(range.get_iterator(), range.get_iterator_end(),
                       that_range.get_iterator(), that_range.get_iterator_end())
                   != range.get_iterator_end();
        }
    };
}
