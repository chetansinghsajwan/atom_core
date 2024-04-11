#pragma once
#include "atom/core/core.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"
#include <algorithm>

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
            using mut_iter_t = typeutils::empty;
            using mut_iter_end_t = typeutils::empty;
        };

        template <typename in_range_t>
            requires is_mut_range<in_range_t>
        struct _mut_aliases_resolver<in_range_t>
        {
            using mut_iter_t = typename in_range_t::mut_iter_t;
            using mut_iter_end_t = typename in_range_t::mut_iter_end_t;
        };

    public:
        using value_t = typename _impl_t::value_t;
        using iter_t = typename _impl_t::iter_t;
        using iter_end_t = typename _impl_t::iter_end_t;
        using mut_iter_t = typename _mut_aliases_resolver<_impl_t>::mut_iter_t;
        using mut_iter_end_t = typename _mut_aliases_resolver<_impl_t>::mut_iter_end_t;
        using std_iter_t = std_iter_wrap_for_atom_iter<iter_t>;
        using std_iter_end_t = std_iter_wrap_for_atom_iter<iter_end_t>;
        using std_mut_iter_t = std_iter_wrap_for_atom_iter<mut_iter_t>;
        using std_mut_iter_end_t = std_iter_wrap_for_atom_iter<mut_iter_end_t>;

    public:
        template <typename this_range_t>
        static constexpr auto get_iter(const this_range_t& this_range) -> iter_t
        {
            return this_range.get_iter();
        }

        template <typename this_range_t>
        static constexpr auto get_iter_end(const this_range_t& this_range) -> iter_end_t
        {
            return this_range.get_iter_end();
        }

        template <typename this_range_t>
        static constexpr auto get_iter_at(const this_range_t& this_range, usize i) -> iter_t
        {
            return this_range.get_iter().next(i);
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iter(this_range_t& this_range) -> mut_iter_t
            requires is_mut_range<this_range_t>
        {
            return this_range.get_mut_iter();
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iter_end(this_range_t& this_range) -> mut_iter_end_t
            requires is_mut_range<this_range_t>
        {
            return this_range.get_mut_iter_end();
        }

        template <typename this_range_t>
        static constexpr auto get_mut_iter_at(this_range_t& this_range, usize i) -> iter_t
            requires is_mut_range<this_range_t>
        {
            return this_range.get_mut_iter().next(i);
        }

        // template <typename this_range_t>
        // static constexpr auto begin(const this_range_t& this_range) -> std_iter_t
        // {
        //     return std_iter_wrap_for_atom_iter(get_iter(this_range));
        // }

        // template <typename this_range_t>
        // static constexpr auto end(const this_range_t& this_range) -> std_iter_end_t
        // {
        //     return std_iter_wrap_for_atom_iter(get_iter_end(this_range));
        // }

        template <typename this_range_t>
        static constexpr auto begin(this_range_t& this_range)
        // -> std_mut_iter_t
        {
            if constexpr (is_mut_range<this_range_t>)
                return std_iter_wrap_for_atom_iter(get_mut_iter(this_range));
            else
                return std_iter_wrap_for_atom_iter(get_iter(this_range));
        }

        template <typename this_range_t>
        static constexpr auto end(this_range_t& this_range)
        // -> std_mut_iter_end_t
        {
            if constexpr (is_mut_range<this_range_t>)
                return std_iter_wrap_for_atom_iter(get_mut_iter_end(this_range));
            else
                return std_iter_wrap_for_atom_iter(get_iter_end(this_range));
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
            return get_data(this_range)[0];
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
            const this_range_t& this_range, const that_value_t& value) -> iter_t
        {
            std_iter_wrap_for_atom_iter std_begin(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_end(get_iter_end(this_range));
            return std::find(std_begin, std_end, value).iter;
        }

        template <typename this_range_t, typename that_range_type>
        static constexpr auto find_range(
            const this_range_t& this_range, const that_range_type& that_range) -> iter_t
        {
            std_iter_wrap_for_atom_iter std_this_begin(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_this_end(get_iter_end(this_range));
            std_iter_wrap_for_atom_iter std_that_begin(that_range.get_iter());
            std_iter_wrap_for_atom_iter std_that_end(that_range.get_iter_end());
            return std::search(std_this_begin, std_this_end, std_that_begin, std_that_end).iter;
        }

        template <typename this_range_t, typename that_range_type>
        static constexpr auto compare(
            const const this_range_t& this_range, that_range_type& that_range) -> i8
        {
            std_iter_wrap_for_atom_iter std_this_begin(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_this_end(get_iter_end(this_range));
            std_iter_wrap_for_atom_iter std_that_begin(that_range.get_iter());
            std_iter_wrap_for_atom_iter std_that_end(that_range.get_iter_end());

            return std::equal(std_this_begin, std_this_end, std_that_begin, std_that_end);
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
            if constexpr (is_jump_iter_pair<iter_t, iter_end_t>)
            {
                return get_iter_end(this_range) - get_iter(this_range);
            }

            usize count = 0;
            for (auto it = get_iter(this_range); it != get_iter_end(this_range); it++)
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
