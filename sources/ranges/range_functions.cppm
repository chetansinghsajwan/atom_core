export module atom_core:ranges.range_functions;

import :core;
import :types;
import :contracts;
import :ranges.iterator_concepts;
import :ranges.range_concepts;
import :ranges.range_conversions;
import :ranges.range_functions_impl;

#include "atom/core/preprocessors.h"

export namespace atom::ranges
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// aliases
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    template <typename range_type>
    using impl_type = range_functions_impl<range_type>;

    template <typename range_type>
    using value_type = typename range_type::value_type;

    // template <typename range_type>
    // using const_iterator_type = typename range_type::const_iterator_type;

    // template <typename range_type>
    // using const_iterator_end_type = typename range_type::const_iterator_end_type;

    // template <typename range_type>
    // using iterator_type = typename range_type::iterator_type;

    // template <typename range_type>
    // using iterator_end_type = typename range_type::iterator_end_type;

    template <typename range_type>
    using view_type = int;

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// basic functionality
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    /// is range empty.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto is_empty(const range_type& range) -> bool
        requires const_range_concept<range_type>
    {
        return impl_type<range_type>::is_empty(range);
    }

    struct is_empty_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> bool
            requires const_range_concept<range_type>
        {
            return ranges::is_empty(range);
        }
    };

    /// ----------------------------------------------------------------------------------------
    /// is range empty.
    /// ----------------------------------------------------------------------------------------
    constexpr auto is_empty() -> is_empty_closure
    {
        return {};
    }

    /// ----------------------------------------------------------------------------------------
    /// is range not empty.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto is_not_empty(const range_type& range) -> bool
        requires const_range_concept<range_type>
    {
        return not impl_type<range_type>::is_empty(range);
    }

    struct is_not_empty_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> bool
            requires const_range_concept<range_type>
        {
            return ranges::is_not_empty(range);
        }
    };

    /// ----------------------------------------------------------------------------------------
    /// is range not empty.
    /// ----------------------------------------------------------------------------------------
    constexpr auto is_not_empty() -> is_not_empty_closure
    {
        return is_not_empty_closure{};
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// iteration
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator(const range_type& range) -> const_iterator_type<range_type>
    // requires const_range_concept<range_type>
    {
        return impl_type<range_type>::get_iterator(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_end(const range_type& range) -> const_iterator_end_type<range_type>
    // requires const_range_concept<range_type>
    {
        return impl_type<range_type>::get_iterator_end(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get iterator to value at index `i`.
    ///
    /// # parameters
    ///
    /// - `i`: index of the value to get iterator at.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_at(
        const range_type& range, usize i) -> const_iterator_type<range_type>
        requires const_array_range_concept<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return impl_type<range_type>::get_iterator_at(range, i);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator(range_type& range) -> iterator_type<range_type>
    // requires range_concept<range_type>
    {
        return impl_type<range_type>::get_iterator(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_end(range_type& range) -> iterator_end_type<range_type>
    // requires range_concept<range_type>
    {
        return impl_type<range_type>::get_iterator_end(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get iterator to value at index `i`.
    ///
    /// # parameters
    ///
    /// - `i`: index of the value to get iterator at.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_at(range_type& range, usize i) -> iterator_type<range_type>
        requires array_range_concept<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return impl_type<range_type>::get_iterator_at(range, i);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// access
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    /// get ptr to underlying arr.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_data(range_type& range) -> value_type<range_type>*
        requires array_range_concept<range_type>
    {
        return impl_type<range_type>::get_data(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get ptr to underlying arr.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_data(const range_type& range) -> const value_type<range_type>*
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::get_data(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// access value by index.
    ///
    /// # parameters
    /// - `i`: index of value to access.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_at(const range_type& range, usize i) -> const value_type<range_type>&
        requires const_array_range_concept<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return impl_type<range_type>::get_at(range, i);
    }

    /// ----------------------------------------------------------------------------------------
    /// access value by index.
    ///
    /// # parameters
    /// - `i`: index of value to access.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_at(range_type& range, usize i) -> value_type<range_type>&
        requires array_range_concept<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return impl_type<range_type>::get_at(range, i);
    }

    /// ----------------------------------------------------------------------------------------
    /// access first value.
    ///
    /// # time complexity
    ///
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_first(range_type& range) -> value_type<range_type>&
        requires array_range_concept<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return impl_type<range_type>::get_first(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// access first value.
    ///
    /// # time complexity
    ///
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_first(const range_type& range) -> const value_type<range_type>&
        requires const_array_range_concept<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return impl_type<range_type>::get_first(range);
    }

    struct get_first_closure
    {
        template <typename range_type>
        constexpr auto operator|(range_type& range) const -> value_type<range_type>&
            requires array_range_concept<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }

        template <typename range_type>
        constexpr auto operator|(range_type&& range) const -> value_type<range_type>&
            requires array_range_concept<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }

        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> const value_type<range_type>&
            requires const_array_range_concept<range_type>
        {
            contract_debug_expects(not ranges::is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }
    };

    /// ----------------------------------------------------------------------------------------
    /// access first value.
    ///
    /// # time complexity
    ///
    /// constant.
    /// ----------------------------------------------------------------------------------------
    constexpr auto get_first() -> get_first_closure
    {
        return get_first_closure{};
    }

    /// ----------------------------------------------------------------------------------------
    /// access last value.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_last(range_type& range) -> value_type<range_type>&
        requires array_range_concept<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return impl_type<range_type>::get_last(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// access last value.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_last(const range_type& range) -> const value_type<range_type>&
        requires const_array_range_concept<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return impl_type<range_type>::get_last(range);
    }

    struct get_last_closure
    {
        template <typename range_type>
        constexpr auto operator|(range_type& range) const -> value_type<range_type>&
            requires array_range_concept<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_last(range);
        }

        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> const value_type<range_type>&
            requires const_array_range_concept<range_type>
        {
            contract_debug_expects(not ranges::is_empty(range), "range is empty.");

            return ranges::get_last(range);
        }
    };

    constexpr auto get_last() -> get_last_closure
    {
        return {};
    }

    /// ----------------------------------------------------------------------------------------
    /// get count of values.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_count(const range_type& range) -> usize
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::get_count(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view(range_type& range, usize from, usize to) -> view_type<range_type>
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::get_view(range, from, to);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view_from(range_type& range, usize from) -> view_type<range_type>
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::get_view_from(range, from);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view_to(range_type& range, usize to) -> view_type<range_type>
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::get_view_to(range, to);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// validation
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    template <typename range_type>
    constexpr auto is_index_in_range(const range_type& range, usize i) -> bool
        requires const_array_range_concept<range_type>
    {
        return impl_type<range_type>::is_index_in_range(range, i);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// search
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type>
    constexpr auto find(
        const range_type& range, const that_value_type& value) -> const_iterator_type<range_type>
        requires const_range_concept<range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     that_value_type>())
    {
        return impl_type<range_type>::find_value(range, value);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename function_type>
    struct find_if_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) -> const_iterator_type<range_type>
            requires const_range_concept<range_type>
        //  and (type_info<function_type>::is_function<bool(const value_type<range_type>&)>())
        {
            return impl_type<range_type>::find_if(range, pred);
        }

        const function_type& pred;
    };

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename function_type>
    constexpr auto find_if(const function_type& pred) -> find_if_closure<function_type>
    {
        return find_if_closure{ pred };
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto find_range(const range_type& range,
        const that_range_type& that_range) -> const_iterator_type<range_type>
        requires const_range_concept<range_type>
                 and const_unidirectional_range_concept<that_range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return impl_type<range_type>::find_range(range, that_range);
    }

    template <typename range_type, typename that_range_type>
    constexpr auto count_any(const range_type& range, const that_range_type& that_range) -> usize
        requires const_range_concept<range_type>
    {
        return impl_type<range_type>::count_any(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type>
    constexpr auto contains(const range_type& range, const that_value_type& value) -> bool
        requires const_range_concept<range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     that_value_type>())
    {
        return impl_type<range_type>::contains(range, value);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename that_value_type>
    struct contains_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) -> bool
            requires const_range_concept<range_type>
                     and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                         that_value_type>())
        {
            return impl_type<range_type>::contains(range, value);
        }

        const that_value_type& value;
    };

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename that_value_type>
    constexpr auto contains(const that_value_type& value) -> contains_closure<that_value_type>
    {
        return contains_closure{ value };
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename function_type>
    struct contains_if_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) -> bool
            requires const_range_concept<range_type>
        //  and (type_info<function_type>::is_function<bool(const value_type<range_type>&)>())
        {
            return impl_type<range_type>::contains_if(range, pred);
        }

        const function_type& pred;
    };

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename function_type>
    constexpr auto contains_if(const function_type& pred) -> contains_if_closure<function_type>
    {
        return contains_if_closure{ pred };
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto contains_range(
        const range_type& range, const that_range_type& that_range) -> bool
        requires(const_range_concept<range_type>
                 and const_unidirectional_range_concept<that_range_type>
                 and type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return impl_type<range_type>::contains(range, that_range);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// comparision
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto compare(const range_type& range, const that_range_type& that_range) -> i8
        requires(const_range_concept<range_type> and const_range_concept<that_range_type>
                 and type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return impl_type<range_type>::compare(range, that_range);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// manipulations
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto can_get_count(const range_type& range) -> bool
        requires const_range_concept<range_type>
    {
        return impl_type<range_type>::can_get_count(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto count_values(const range_type& range) -> usize
        requires const_range_concept<range_type>
    {
        return impl_type<range_type>::count_values(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename value1_type>
    constexpr auto write_values(range_type& range, value1_type& val)
        -> range_type& requires(range_concept<range_type>and
                type_info<value_type<range_type>>::template is_assignable_from<value1_type>()) {
        impl_type<range_type>::write_values(range, val);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    ATOM_PRAGMA_OPTIMIZE_OFF

        template <typename range_type, typename value1_type>
        constexpr auto write_values_no_optimize(range_type& range, value1_type& val)
            -> range_type& requires(range_concept<range_type>and
                    type_info<value_type<range_type>>::template is_assignable_from<value1_type>()) {
            impl_type<range_type>::write_values_no_optimize(range, val);
            return range;
        }

    ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename range_type>
        constexpr auto shift_fwd(range_type& range,
            usize steps) -> range_type& requires(range_concept<range_type>and
                                 type_info<value_type<range_type>>::is_move_assignable()) {
            impl_type<range_type>::shift_fwd(range, steps);
            return range;
        }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto shift_bwd(range_type& range, usize steps)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_move_assignable()) {
        impl_type<range_type>::shift_bwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto shift_by(range_type& range, isize steps)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_swappable()) {
        impl_type<range_type>::shift_by(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_fwd(range_type& range, usize steps)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_swappable()) {
        impl_type<range_type>::rotate_fwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_bwd(range_type& range, usize steps)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_swappable()) {
        impl_type<range_type>::rotate_bwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_by(range_type& range, isize steps)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_swappable()) {
        impl_type<range_type>::rotate_by(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto destroy_values(range_type& range)
        -> range_type& requires(
            range_concept<range_type>and type_info<value_type<range_type>>::is_destructible()) {
        impl_type<range_type>::destroy_values(range);
        return range;
    }
}

export namespace atom
{
    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto begin(const range_type& range) -> ranges::const_iterator_type<range_type>
        requires ranges::const_range_concept<range_type>
    {
        return range.get_iterator();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto end(const range_type& range) -> ranges::const_iterator_end_type<range_type>
        requires ranges::const_range_concept<range_type>
    {
        return range.get_iterator_end();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto begin(range_type& range) -> ranges::iterator_type<range_type>
        requires ranges::range_concept<range_type>
    {
        return range.get_iterator();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto end(range_type& range) -> ranges::iterator_end_type<range_type>
        requires ranges::range_concept<range_type>
    {
        return range.get_iterator_end();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto operator==(const range_type& range, const that_range_type& that_range) -> bool
        requires(
            ranges::const_range_concept<range_type> and ranges::const_range_concept<that_range_type>
            and type_info<ranges::value_type<range_type>>::template is_equality_comparable_with<
                ranges::value_type<that_range_type>>())
    {
        return ranges::impl_type<range_type>::is_eq(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto operator!=(const range_type& range, const that_range_type& that_range) -> bool
        requires ranges::const_range_concept<range_type>
                 and ranges::const_range_concept<that_range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<
                             ranges::value_type<that_range_type>>())
    {
        return not ranges::impl_type<range_type>::is_eq(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type, usize that_count>
    constexpr auto operator==(
        const range_type& range, const that_value_type (&that_arr)[that_count]) -> bool
        requires ranges::const_range_concept<range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<that_value_type>())
    {
        return ranges::impl_type<range_type>::is_eq(range, ranges::from(that_arr));
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type, usize that_count>
    constexpr auto operator!=(
        const range_type& range, const that_value_type (&that_arr)[that_count]) -> bool
        requires ranges::const_range_concept<range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<that_value_type>())
    {
        return not ranges::impl_type<range_type>::is_eq(range, ranges::from(that_arr));
    }

    template <typename value_type0, typename value1_type>
    constexpr auto operator|(const value1_type& value1, const value_type0& value0) -> decltype(auto)
        requires requires(
            const value_type0& value0, const value1_type& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }

    template <typename value_type0, typename value1_type>
    constexpr auto operator|(value1_type& value1, value_type0& value0) -> decltype(auto)
        requires requires(value_type0& value0, value1_type& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }

    template <typename value_type0, typename value1_type>
    constexpr auto operator|(value1_type&& value1, value_type0&& value0) -> decltype(auto)
        requires requires(value_type0&& value0, value1_type&& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }
}
