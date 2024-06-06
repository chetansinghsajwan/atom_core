export module atom.core:ranges.range_extensions_impl;

import std;
import :core;
import :types;
import :ranges.iterator_requirements;
import :ranges.range_requirements;

namespace atom
{
    template <typename range_t>
    class _range_extensions_impl
    {
    protected:
        using _impl_t = range_t;

    private:
        template <typename in_range_t>
        struct _mut_aliases_resolver
        {
            using mut_iterator_t = typeutils::empty;
            using mut_iterator_end_t = typeutils::empty;
        };

        template <typename in_range_t>
            requires ranges::is_mut_range<in_range_t>
        struct _mut_aliases_resolver<in_range_t>
        {
            using mut_iterator_t = typename in_range_t::mut_iterator_t;
            using mut_iterator_end_t = typename in_range_t::mut_iterator_end_t;
        };

    public:
        using value_t = typename _impl_t::value_t;
        using iterator_t = typename _impl_t::iterator_t;
        using iterator_end_t = typename _impl_t::iterator_end_t;
        using mut_iterator_t = typename _mut_aliases_resolver<_impl_t>::mut_iterator_t;
        using mut_iterator_end_t = typename _mut_aliases_resolver<_impl_t>::mut_iterator_end_t;

    public:
        template <typename this_range_t>
        static constexpr auto get_iterator(const this_range_t& this_range) -> iterator_t
        {
            return this_range.get_iterator();
        }

        template <typename this_range_t>
        static constexpr auto get_iterator_end(const this_range_t& this_range) -> iterator_end_t
        {
            return this_range.get_iterator_end();
        }

        template <typename this_range_t>
        static constexpr auto get_iterator_at(const this_range_t& this_range, usize i) -> iterator_t
        {
            return this_range.get_iterator().next(i);
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iterator(this_range_t& this_range) -> mut_iterator_t
            requires ranges::is_mut_range<this_range_t>
        {
            return this_range.get_mut_iterator();
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iterator_end(this_range_t& this_range) -> mut_iterator_end_t
            requires ranges::is_mut_range<this_range_t>
        {
            return this_range.get_mut_iterator_end();
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iterator_at(this_range_t& this_range, usize i) -> iterator_t
            requires ranges::is_mut_range<this_range_t>
        {
            return this_range.get_mut_iterator().next(i);
        }

        // template <typename this_range_t>
        // static constexpr auto begin(const this_range_t& this_range) -> std_iterator_t
        // {
        //     return get_iterator(this_range);
        // }

        // template <typename this_range_t>
        // static constexpr auto end(const this_range_t& this_range) -> std_iterator_end_t
        // {
        //     return get_iterator_end(this_range);
        // }

        template <typename this_range_t>
        static constexpr auto begin(this_range_t& this_range)
        // -> std_mut_iterator_t
        {
            if constexpr (ranges::is_mut_range<this_range_t>)
                return get_mut_iterator(this_range);
            else
                return get_iterator(this_range);
        }

        template <typename this_range_t>
        static constexpr auto end(this_range_t& this_range)
        // -> std_mut_iterator_end_t
        {
            if constexpr (ranges::is_mut_range<this_range_t>)
                return get_mut_iterator_end(this_range);
            else
                return get_iterator_end(this_range);
        }

        template <typename this_range_t>
        static constexpr auto get_data(const this_range_t& this_range) -> const value_t*
        {
            return this_range.get_data();
        }

        template <typename this_range_t>
        static constexpr auto get_mut_data(this_range_t& this_range) -> value_t*
        {
            return this_range.get_mut_data();
        }

        template <typename this_range_t>
        static constexpr auto get_at(const this_range_t& this_range, usize i) -> const value_t&
        {
            return get_data(this_range)[i];
        }

        template <typename this_range_t>
        static constexpr auto get_mut_at(this_range_t& this_range, usize i) -> value_t&
        {
            return get_mut_data(this_range)[i];
        }

        template <typename this_range_t>
        static constexpr auto get_front(const this_range_t& this_range) -> const value_t&
        {
            return get_data(this_range)[0];
        }

        template <typename this_range_t>
        static constexpr auto get_front_mut(this_range_t& this_range) -> value_t&
        {
            return get_mut_data(this_range)[0];
        }

        template <typename this_range_t>
        static constexpr auto get_back(const this_range_t& this_range) -> const value_t&
        {
            return get_data(this_range)[get_count(this_range) - 1];
        }

        template <typename this_range_t>
        static constexpr auto get_back_mut(this_range_t& this_range) -> value_t&
        {
            return get_data(this_range)[get_count(this_range) - 1];
        }

        template <typename this_range_t>
        static constexpr auto get_count(const this_range_t& this_range) -> usize
        {
            return this_range.get_count();
        }

        template <typename this_range_t>
        static constexpr auto is_empty(const this_range_t& this_range) -> bool
        {
            return get_count(this_range) == 0;
        }

        template <typename this_range_t, typename that_value_t>
        static constexpr auto find_elem(
            const this_range_t& this_range, const that_value_t& value) -> iterator_t
        {
            auto begin = get_iterator(this_range);
            auto end = get_iterator_end(this_range);

            return std::find(begin, end, value).iterator;
        }

        template <typename this_range_t, typename that_range_type>
        static constexpr auto find_range(
            const this_range_t& this_range, const that_range_type& that_range) -> iterator_t
        {
            auto this_begin = get_iterator(this_range);
            auto this_end = get_iterator_end(this_range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return std::search(this_begin, this_end, that_begin, that_end).iterator;
        }

        template <typename this_range_t, typename that_range_type>
        static constexpr auto compare(
            const this_range_t& this_range, that_range_type& that_range) -> i8
        {
            auto this_begin = get_iterator(this_range);
            auto this_end = get_iterator_end(this_range);
            auto that_begin = that_range.get_iterator();
            auto that_end = that_range.get_iterator_end();

            return std::equal(this_begin, this_end, that_begin, that_end);
        }

        template <typename this_range_t, typename that_range_type>
        static constexpr auto is_eq(
            const this_range_t& this_range, const that_range_type& that_range) -> bool
        {
            return compare(this_range, that_range) == 0;
        }

        template <typename this_range_t>
        static constexpr auto count_elems(const this_range_t& this_range) -> usize
        {
            if constexpr (is_random_access_iterator_pair<iterator_t, iterator_end_t>)
            {
                return get_iterator_end(this_range) - get_iterator(this_range);
            }

            usize count = 0;
            for (auto it = get_iterator(this_range); it != get_iterator_end(this_range); it++)
                count++;

            return count;
        }

        template <typename this_range_t>
        static constexpr auto is_index_in_range(const this_range_t& this_range, usize i) -> bool
        {
            return i < get_count(this_range);
        }
    };
}
