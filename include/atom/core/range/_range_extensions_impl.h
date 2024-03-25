#pragma once
#include "atom/core/core.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"

namespace atom
{
    template <typename range_t>
    class _range_extensions_impl
    {
    protected:
        using _impl_t = range_t;

    public:
        using value_t = typename _impl_t::value_t;
        using iter_t = typename _impl_t::iter_t;
        using iter_end_t = typename _impl_t::iter_end_t;

    public:
        template <typename this_range_type>
        static constexpr auto get_iter(const this_range_type& this_range) -> iter_t
        {
            return this_range.get_iter();
        }

        template <typename this_range_type>
        static constexpr auto get_iter_end(const this_range_type& this_range) -> iter_end_t
        {
            return this_range.get_iter_end();
        }

        template <typename this_range_type, typename that_elem_type>
        static constexpr auto find_elem(const this_range_type& this_range, const that_elem_type& el) -> iter_t
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end(this_range));
            return std::find(std_iter, std_iter_end, el).iter;
        }

        template <typename this_range_type, typename that_range_type>
        static constexpr auto find_range(const this_range_type& this_range, const that_range_type& that_range) -> iter_t
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end(this_range));
            std_iter_wrap_for_atom_iter std_iter1(that_range.get_iter(this_range));
            std_iter_wrap_for_atom_iter std_iter_end1(that_range.get_iter_end(this_range));
            return std::search(std_iter, std_iter_end, std_iter1, std_iter_end1).iter;
        }

        template <typename this_range_type, typename that_range_type>
        static constexpr auto compare(const const this_range_type& this_range, that_range_type& that_range) -> i8
        {
            std_iter_wrap_for_atom_iter std_iter(get_iter(this_range));
            std_iter_wrap_for_atom_iter std_iter_end(get_iter_end(this_range));
            std_iter_wrap_for_atom_iter std_iter1(that_range.get_iter(this_range));
            std_iter_wrap_for_atom_iter std_iter_end1(that_range.get_iter_end(this_range));

            return std::equal(std_iter, std_iter_end, std_iter1, std_iter_end1);
        }

        template <typename this_range_type>
        static constexpr auto get_count(const this_range_type& this_range) -> usize
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
    };
}