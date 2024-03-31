#pragma once
#include "_range_extensions_impl.h"
#include "atom/core/range/iter_requirements.h"
#include "atom/core/range/range_requirements.h"
#include "atom/core/range/std_iter_wrap_for_atom_iter.h"

namespace atom
{
    class range_extensions
    {
        using this_type = range_extensions;

    public:
        template <typename this_range_type>
        using get_impl_type = _range_extensions_impl<this_range_type>;

        template <typename range_type>
        using get_value_type = typename get_impl_type<range_type>::value_t;

        template <typename this_range_type>
        using get_iter_type = typename this_range_type::iter_t;

        template <typename this_range_type>
        using get_iter_end_type = typename this_range_type::iter_end_t;

        template <typename this_range_type>
        using get_mut_iter_type = typename this_range_type::mut_iter_t;

        template <typename this_range_type>
        using get_mut_iter_end_type = typename this_range_type::mut_iter_end_t;

        template <typename this_range_type>
        using get_std_iter_type = std_iter_wrap_for_atom_iter<get_iter_type<this_range_type>>;

        template <typename this_range_type>
        using get_std_iter_end_type =
            std_iter_wrap_for_atom_iter<get_iter_end_type<this_range_type>>;

        template <typename this_range_type>
        using get_std_mut_iter_type =
            std_iter_wrap_for_atom_iter<get_mut_iter_type<this_range_type>>;

        template <typename this_range_type>
        using get_std_mut_iter_end_type =
            std_iter_wrap_for_atom_iter<get_mut_iter_end_type<this_range_type>>;

        template <typename this_range_type>
        using get_view_type = int;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_iter(
        //     this const this_range_type& this_range) -> get_iter_type<this_range_type>
        //     // requires is_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_iter(this_range);
        // }

        // /// ----------------------------------------------------------------------------------------
        // ///
        // /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_iter_end(
        //     this const this_range_type& this_range) -> get_iter_end_type<this_range_type>
        //     // requires is_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_iter_end(this_range);
        // }

        // /// ----------------------------------------------------------------------------------------
        // /// get iter to element at index `i`.
        // ///
        // /// # parameters
        // ///
        // /// - `i`: index of the element to get iter at.
        // /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_iter_at(
        //     this const this_range_type& this_range, usize i) -> get_iter_type<this_range_type>
        //     // requires is_array_range<this_range_type>
        // {
        //     ATOM_EXPECTS(this_range.this_type::is_index_in_range(i), "index is out of range.");

        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_iter_at(this_range, i);
        // }

        // /// ----------------------------------------------------------------------------------------
        // ///
        // /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_mut_iter(
        //     this this_range_type& this_range) -> get_mut_iter_type<this_range_type>
        //     // requires is_mut_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_mut_iter(this_range);
        // }

        // /// ----------------------------------------------------------------------------------------
        // ///
        // /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_mut_iter_end(
        //     this this_range_type& this_range) -> get_mut_iter_end_type<this_range_type>
        //     // requires is_mut_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_mut_iter_end(this_range);
        // }

        // /// ----------------------------------------------------------------------------------------
        // /// get iter to element at index `i`.
        // ///
        // /// # parameters
        // ///
        // /// - `i`: index of the element to get iter at.
        // /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto get_mut_iter_at(
        //     this this_range_type& this_range, usize i) -> get_mut_iter_type<this_range_type>
        //     // requires is_mut_array_range<this_range_type>
        // {
        //     ATOM_EXPECTS(this_range.this_type::is_index_in_range(i), "index is out of range.");

        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::get_mut_iter_at(this_range, i);
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto begin(
        //     this const this_range_type& this_range) -> get_std_iter_type<this_range_type>
        // requires is_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::begin(this_range);
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        // template <typename this_range_type>
        // constexpr auto end(
        //     this const this_range_type& this_range) -> get_std_iter_end_type<this_range_type>
        // requires is_range<this_range_type>
        // {
        //     using impl_type = get_impl_type<this_range_type>;
        //     return impl_type::end(this_range);
        // }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto begin(this this_range_type& this_range)
        //  -> get_std_mut_iter_type<this_range_type>
        // requires is_mut_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::begin(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto end(this this_range_type& this_range)
        //  -> get_std_mut_iter_end_type<this_range_type>
        // requires is_mut_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::end(this_range);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get ptr to underlying arr.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_data(
            this const this_range_type& this_range) -> const get_value_type<this_range_type>*
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_data(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// get ptr to underlying arr.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_mut_data(
            this this_range_type& this_range) -> get_value_type<this_range_type>*
            requires is_mut_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_mut_data(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_at(this const this_range_type& this_range,
            usize i) -> const get_value_type<this_range_type>&
            requires is_array_range<this_range_type>
        {
            ATOM_EXPECTS(this_range.this_type::is_index_in_range(i), "index is out of range.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_at(this_range, i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_mut_at(
            this this_range_type& this_range, usize i) -> get_value_type<this_range_type>&
            requires is_mut_array_range<this_range_type>
        {
            ATOM_EXPECTS(this_range.this_type::is_index_in_range(i), "index is out of range.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_mut_at(this_range, i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto operator[](this const this_range_type& this_range,
            usize i) -> const get_value_type<this_range_type>&
            requires is_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(
                this_range.this_type::is_index_in_range(i), "index is out of range.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_at(this_range, i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access element by index.
        ///
        /// # parameters
        /// - `i`: index of element to access.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto operator[](
            this this_range_type& this_range, usize i) -> get_value_type<this_range_type>&
            requires is_mut_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(
                this_range.this_type::is_index_in_range(i), "index is out of range.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_mut_at(this_range, i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_front(
            this const this_range_type& this_range) -> const get_value_type<this_range_type>&
            requires is_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(not this_range.this_type::is_empty(), "range is empty.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_front(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_mut_front(
            this this_range_type& this_range) -> get_value_type<this_range_type>&
            requires is_mut_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(not this_range.this_type::is_empty(), "range is empty.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_mut_front(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_back(
            this const this_range_type& this_range) -> const get_value_type<this_range_type>&
            requires is_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(not this_range.this_type::is_empty(), "range is empty.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_back(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_mut_back(
            this this_range_type& this_range) -> get_value_type<this_range_type>&
            requires is_mut_array_range<this_range_type>
        {
            ATOM_DEBUG_EXPECTS(not this_range.this_type::is_empty(), "range is empty.");

            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_mut_back(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// get count of elements.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_count(this const this_range_type& this_range) -> usize
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_count(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        /// is range empty.
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto is_empty(this const this_range_type& this_range) -> bool
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::is_empty(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_view(this this_range_type& this_range, usize from,
            usize to) -> get_view_type<this_range_type>
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_view(this_range, from, to);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_view_from(
            this this_range_type& this_range, usize from) -> get_view_type<this_range_type>
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_view_from(this_range, from);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto get_view_to(
            this this_range_type& this_range, usize to) -> get_view_type<this_range_type>
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::get_view_to(this_range, to);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// validation
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        template <typename this_range_type>
        constexpr auto is_index_in_range(this const this_range_type& this_range, usize i) -> bool
            requires is_array_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::is_index_in_range(this_range, i);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// search
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_value_type>
        constexpr auto find(this const this_range_type& this_range,
            const that_value_type& value) -> get_iter_type<this_range_type>
            requires is_range<this_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<that_value_type>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::find_elem(this_range, value);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_range_type>
        constexpr auto find_range(this const this_range_type& this_range,
            const that_range_type& that_range) -> get_iter_type<this_range_type>
            requires is_range<this_range_type> and is_fwd_range<that_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<
                                 typename that_range_type::value_t>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::find_range(this_range, that_range);
        }

        template <typename this_range_type, typename that_range_type>
        constexpr auto count_any(
            this const this_range_type& this_range, const that_range_type& that_range) -> usize
            requires is_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::count_any(this_range, that_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_value_type>
        constexpr auto contains(
            this const this_range_type& this_range, const that_value_type& value) -> bool
            requires is_range<this_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<that_value_type>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::contains(this_range, value);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_range_type>
        constexpr auto contains(
            this const this_range_type& this_range, const that_range_type& that_range) -> bool
            requires is_range<this_range_type> and is_fwd_range<that_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<
                                 typename that_range_type::value_t>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::contains(this_range, that_range);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// comparision
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_range_type>
        constexpr auto compare(
            this const this_range_type& this_range, const that_range_type& that_range) -> i8
            requires is_range<this_range_type> and is_range<that_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<
                                 typename that_range_type::value_t>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::compare(this_range, that_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename that_range_type>
        constexpr auto is_eq(
            this const this_range_type& this_range, const that_range_type& that_range) -> bool
            requires is_range<this_range_type> and is_range<that_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::
                             template is_equality_comparable_with<
                                 typename that_range_type::value_t>)
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::is_eq(this_range, that_range);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// manipulations
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto can_get_count(this const this_range_type& this_range) -> bool
            requires is_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::can_get_count(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto count_elems(this const this_range_type& this_range) -> usize
            requires is_range<this_range_type>
        {
            using impl_type = get_impl_type<this_range_type>;
            return impl_type::count_elems(this_range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type, typename value_type1>
        constexpr auto write_elems(
            this this_range_type& this_range, value_type1& val) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::template is_assignable_from<
                         value_type1>)
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::write_elems(this_range, val);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <typename this_range_type, typename value_type1>
        constexpr auto write_elems_no_optimize(
            this this_range_type& this_range, value_type1& val) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and (typeinfo<get_value_type<this_range_type>>::template is_assignable_from<
                         value_type1>)
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::write_elems_no_optimize(this_range, val);
            return this_range;
        }

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto shift_fwd(this this_range_type& this_range, usize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_move_assignable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::shift_fwd(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto shift_bwd(this this_range_type& this_range, usize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_move_assignable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::shift_bwd(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto shift_by(this this_range_type& this_range, isize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_swappable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::shift_by(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto rotate_fwd(this this_range_type& this_range, usize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_swappable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::rotate_fwd(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto rotate_bwd(this this_range_type& this_range, usize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_swappable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::rotate_bwd(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto rotate_by(this this_range_type& this_range, isize steps) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_swappable
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::rotate_by(this_range, steps);
            return this_range;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_range_type>
        constexpr auto destroy_elems(this this_range_type& this_range) -> this_range_type&
            requires is_mut_range<this_range_type>
                     and typeinfo<get_value_type<this_range_type>>::is_destructible
        {
            using impl_type = get_impl_type<this_range_type>;
            impl_type::destroy_elems(this_range);
            return this_range;
        }
    };
}
