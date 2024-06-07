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
            using mut_iterator_type = typeutils::empty;
            using mut_iterator_end_type = typeutils::empty;
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
        using mut_iterator_end_type = typename _mut_aliases_resolver<_impl_type>::mut_iterator_end_type;

    public:
        template <typename this_range_type>
        static constexpr auto get_iterator(const this_range_type& this_range) -> iterator_type
        {
            return this_range.get_iterator();
        }

        template <typename this_range_type>
        static constexpr auto get_iterator_end(const this_range_type& this_range) -> iterator_end_type
        {
            return this_range.get_iterator_end();
        }

        template <typename this_range_type>
        static constexpr auto get_iterator_at(const this_range_type& this_range, usize i) -> iterator_type
        {
            return this_range.get_iterator().next(i);
        }

        template <typename this_range_type>
        static constexpr auto get_mut_iterator(this_range_type& this_range) -> mut_iterator_type
            requires ranges::is_mut_range<this_range_type>
        {
            return this_range.get_mut_iterator();
        }

        template <typename this_range_type>
        static constexpr auto get_mut_iterator_end(this_range_type& this_range) -> mut_iterator_end_type
            requires ranges::is_mut_range<this_range_type>
        {
            return this_range.get_mut_iterator_end();
        }

        template <typename this_range_type>
        static constexpr auto get_mut_iterator_at(this_range_type& this_range, usize i) -> iterator_type
            requires ranges::is_mut_range<this_range_type>
        {
            return this_range.get_mut_iterator().next(i);
        }

        // template <typename this_range_type>
        // static constexpr auto begin(const this_range_type& this_range) -> std_iterator_type
        // {
        //     return get_iterator(this_range);
        // }

        // template <typename this_range_type>
        // static constexpr auto end(const this_range_type& this_range) -> std_iterator_end_type
        // {
        //     return get_iterator_end(this_range);
        // }

        template <typename this_range_type>
        static constexpr auto begin(this_range_type& this_range)
        // -> std_mut_iterator_type
        {
            if constexpr (ranges::is_mut_range<this_range_type>)
                return get_mut_iterator(this_range);
            else
                return get_iterator(this_range);
        }

        template <typename this_range_type>
        static constexpr auto end(this_range_type& this_range)
        // -> std_mut_iterator_end_type
        {
            if constexpr (ranges::is_mut_range<this_range_type>)
                return get_mut_iterator_end(this_range);
            else
                return get_iterator_end(this_range);
        }

        template <typename this_range_type>
        static constexpr auto get_data(const this_range_type& this_range) -> const value_type*
        {
            return this_range.get_data();
        }

        template <typename this_range_type>
        static constexpr auto get_mut_data(this_range_type& this_range) -> value_type*
        {
            return this_range.get_mut_data();
        }

        template <typename this_range_type>
        static constexpr auto get_at(const this_range_type& this_range, usize i) -> const value_type&
        {
            return get_data(this_range)[i];
        }

        template <typename this_range_type>
        static constexpr auto get_mut_at(this_range_type& this_range, usize i) -> value_type&
        {
            return get_mut_data(this_range)[i];
        }

        template <typename this_range_type>
        static constexpr auto get_front(const this_range_type& this_range) -> const value_type&
        {
            return get_data(this_range)[0];
        }

        template <typename this_range_type>
        static constexpr auto get_front_mut(this_range_type& this_range) -> value_type&
        {
            return get_mut_data(this_range)[0];
        }

        template <typename this_range_type>
        static constexpr auto get_back(const this_range_type& this_range) -> const value_type&
        {
            return get_data(this_range)[get_count(this_range) - 1];
        }

        template <typename this_range_type>
        static constexpr auto get_back_mut(this_range_type& this_range) -> value_type&
        {
            return get_mut_data(this_range)[get_count(this_range) - 1];
        }

        template <typename this_range_type>
        static constexpr auto get_count(const this_range_type& this_range) -> usize
        {
            return this_range.get_count();
        }

        template <typename this_range_type>
        static constexpr auto is_empty(const this_range_type& this_range) -> bool
        {
            return get_count(this_range) == 0;
        }

        template <typename this_range_type, typename that_value_type>
        static constexpr auto find_elem(
            const this_range_type& this_range, const that_value_type& value) -> iterator_type
        {
            auto begin = get_iterator(this_range);
            auto end = get_iterator_end(this_range);

            return std::find(begin, end, value).iterator;
        }

        template <typename this_range_type, typename that_range_type>
        static constexpr auto find_range(
            const this_range_type& this_range, const that_range_type& that_range) -> iterator_type
        {
            auto this_begin = get_iterator(this_range);
            auto this_end = get_iterator_end(this_range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return std::search(this_begin, this_end, that_begin, that_end).iterator;
        }

        template <typename this_range_type, typename that_range_type>
        static constexpr auto compare(
            const this_range_type& this_range, that_range_type& that_range) -> i8
        {
            auto this_begin = get_iterator(this_range);
            auto this_end = get_iterator_end(this_range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return std::equal(this_begin, this_end, that_begin, that_end);
        }

        template <typename this_range_type, typename that_range_type>
        static constexpr auto is_eq(
            const this_range_type& this_range, const that_range_type& that_range) -> bool
        {
            return compare(this_range, that_range) == 0;
        }

        template <typename this_range_type>
        static constexpr auto count_elems(const this_range_type& this_range) -> usize
        {
            if constexpr (ranges::is_random_access_iterator_pair<iterator_type, iterator_end_type>)
            {
                return get_iterator_end(this_range) - get_iterator(this_range);
            }

            usize count = 0;
            for (auto it = get_iterator(this_range); it != get_iterator_end(this_range); it++)
                count++;

            return count;
        }

        template <typename this_range_type>
        static constexpr auto is_index_in_range(const this_range_type& this_range, usize i) -> bool
        {
            return i < get_count(this_range);
        }
    };
}
