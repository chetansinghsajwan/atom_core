export module atom_core:ranges.range_extensions;

import :core;
import :types;
import :contracts;
import :ranges.range_extensions_impl;
import :ranges.iterator_requirements;
import :ranges.range_requirements;
import :ranges.range_conversions;

#include "atom/core/preprocessors.h"

export namespace atom::ranges
{
    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// aliases
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    template <typename range_type>
    using _impl_type = _range_extensions_impl<range_type>;

    template <typename range_type>
    using value_type = typename range_type::value_type;

    template <typename range_type>
    using iterator_type = typename range_type::iterator_type;

    template <typename range_type>
    using iterator_end_type = typename range_type::iterator_end_type;

    template <typename range_type>
    using mut_iterator_type = typename range_type::mut_iterator_type;

    template <typename range_type>
    using mut_iterator_end_type = typename range_type::mut_iterator_end_type;

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
        requires is_range<range_type>
    {
        return _impl_type<range_type>::is_empty(range);
    }

    struct is_empty_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> bool
            requires is_range<range_type>
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
        requires is_range<range_type>
    {
        return not _impl_type<range_type>::is_empty(range);
    }

    struct is_not_empty_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> bool
            requires is_range<range_type>
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
    constexpr auto get_iterator(const range_type& range) -> iterator_type<range_type>
        requires is_range<range_type>
    {
        return _impl_type<range_type>::get_iterator(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_end(const range_type& range) -> iterator_end_type<range_type>
        requires is_range<range_type>
    {
        return _impl_type<range_type>::get_iterator_end(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get iterator to element at index `i`.
    ///
    /// # parameters
    ///
    /// - `i`: index of the element to get iterator at.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_at(const range_type& range, usize i) -> iterator_type<range_type>
        requires is_array_range<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return _impl_type<range_type>::get_iterator_at(range, i);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator(range_type& range) -> mut_iterator_type<range_type>
        requires is_mut_range<range_type>
    {
        return _impl_type<range_type>::get_iterator(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_end(range_type& range) -> mut_iterator_end_type<range_type>
        requires is_mut_range<range_type>
    {
        return _impl_type<range_type>::get_iterator_end(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get iterator to element at index `i`.
    ///
    /// # parameters
    ///
    /// - `i`: index of the element to get iterator at.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_iterator_at(range_type& range, usize i) -> mut_iterator_type<range_type>
        requires is_mut_array_range<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return _impl_type<range_type>::get_iterator_at(range, i);
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
        requires is_mut_array_range<range_type>
    {
        return _impl_type<range_type>::get_data(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// get ptr to underlying arr.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_data(const range_type& range) -> const value_type<range_type>*
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::get_data(range);
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
    template <typename range_type>
    constexpr auto get_at(const range_type& range, usize i) -> const value_type<range_type>&
        requires is_array_range<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return _impl_type<range_type>::get_at(range, i);
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
    template <typename range_type>
    constexpr auto get_at(range_type& range, usize i) -> value_type<range_type>&
        requires is_mut_array_range<range_type>
    {
        contract_expects(is_index_in_range(range, i), "index is out of range.");

        return _impl_type<range_type>::get_at(range, i);
    }

    /// ----------------------------------------------------------------------------------------
    /// access first element.
    ///
    /// # time complexity
    ///
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_first(range_type& range) -> value_type<range_type>&
        requires is_mut_array_range<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return _impl_type<range_type>::get_first(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// access first element.
    ///
    /// # time complexity
    ///
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_first(const range_type& range) -> const value_type<range_type>&
        requires is_array_range<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return _impl_type<range_type>::get_first(range);
    }

    struct get_first_closure
    {
        template <typename range_type>
        constexpr auto operator|(range_type& range) const -> value_type<range_type>&
            requires is_mut_array_range<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }

        template <typename range_type>
        constexpr auto operator|(range_type&& range) const -> value_type<range_type>&
            requires is_mut_array_range<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }

        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> const value_type<range_type>&
            requires is_array_range<range_type>
        {
            contract_debug_expects(not ranges::is_empty(range), "range is empty.");

            return ranges::get_first(range);
        }
    };

    /// ----------------------------------------------------------------------------------------
    /// access first element.
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
    /// access last element.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_last(range_type& range) -> value_type<range_type>&
        requires is_mut_array_range<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return _impl_type<range_type>::get_last(range);
    }

    /// ----------------------------------------------------------------------------------------
    /// access last element.
    ///
    /// # time complexity
    /// constant.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_last(const range_type& range) -> const value_type<range_type>&
        requires is_array_range<range_type>
    {
        contract_debug_expects(not is_empty(range), "range is empty.");

        return _impl_type<range_type>::get_last(range);
    }

    struct get_last_closure
    {
        template <typename range_type>
        constexpr auto operator|(range_type& range) const -> value_type<range_type>&
            requires is_mut_array_range<range_type>
        {
            contract_debug_expects(not is_empty(range), "range is empty.");

            return ranges::get_last(range);
        }

        template <typename range_type>
        constexpr auto operator|(const range_type& range) const -> const value_type<range_type>&
            requires is_array_range<range_type>
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
    /// get count of elements.
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_count(const range_type& range) -> usize
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::get_count(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view(range_type& range, usize from, usize to) -> view_type<range_type>
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::get_view(range, from, to);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view_from(range_type& range, usize from) -> view_type<range_type>
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::get_view_from(range, from);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto get_view_to(range_type& range, usize to) -> view_type<range_type>
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::get_view_to(range, to);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// validation
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////

    template <typename range_type>
    constexpr auto is_index_in_range(const range_type& range, usize i) -> bool
        requires is_array_range<range_type>
    {
        return _impl_type<range_type>::is_index_in_range(range, i);
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
        const range_type& range, const that_value_type& value) -> iterator_type<range_type>
        requires is_range<range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     that_value_type>())
    {
        return _impl_type<range_type>::find_elem(range, value);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename function_type>
    struct find_if_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) -> iterator_type<range_type>
            requires is_range<range_type>
        //  and (type_info<function_type>::is_function<bool(const value_type<range_type>&)>)
        {
            return _impl_type<range_type>::find_if(range, pred);
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
    constexpr auto find_range(
        const range_type& range, const that_range_type& that_range) -> iterator_type<range_type>
        requires is_range<range_type> and is_unidirectional_range<that_range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return _impl_type<range_type>::find_range(range, that_range);
    }

    template <typename range_type, typename that_range_type>
    constexpr auto count_any(const range_type& range, const that_range_type& that_range) -> usize
        requires is_range<range_type>
    {
        return _impl_type<range_type>::count_any(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type>
    constexpr auto contains(const range_type& range, const that_value_type& value) -> bool
        requires is_range<range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     that_value_type>())
    {
        return _impl_type<range_type>::contains(range, value);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename that_value_type>
    struct contains_closure
    {
        template <typename range_type>
        constexpr auto operator|(const range_type& range) -> bool
            requires is_range<range_type>
                     and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                         that_value_type>())
        {
            return _impl_type<range_type>::contains(range, value);
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
            requires is_range<range_type>
        //  and (type_info<function_type>::is_function<bool(const value_type<range_type>&)>)
        {
            return _impl_type<range_type>::contains_if(range, pred);
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
        requires is_range<range_type> and is_unidirectional_range<that_range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return _impl_type<range_type>::contains(range, that_range);
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
        requires is_range<range_type> and is_range<that_range_type>
                 and (type_info<value_type<range_type>>::template is_equality_comparable_with<
                     typename that_range_type::value_type>())
    {
        return _impl_type<range_type>::compare(range, that_range);
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
        requires is_range<range_type>
    {
        return _impl_type<range_type>::can_get_count(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto count_elems(const range_type& range) -> usize
        requires is_range<range_type>
    {
        return _impl_type<range_type>::count_elems(range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename value_type1>
    constexpr auto write_elems(range_type& range, value_type1& val) -> range_type&
        requires is_mut_range<range_type>
                 and (type_info<value_type<range_type>>::template is_assignable_from<value_type1>())
    {
        _impl_type<range_type>::write_elems(range, val);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    ATOM_PRAGMA_OPTIMIZE_OFF

    template <typename range_type, typename value_type1>
    constexpr auto write_elems_no_optimize(range_type& range, value_type1& val) -> range_type&
        requires is_mut_range<range_type>
                 and (type_info<value_type<range_type>>::template is_assignable_from<value_type1>())
    {
        _impl_type<range_type>::write_elems_no_optimize(range, val);
        return range;
    }

    ATOM_PRAGMA_OPTIMIZE_ON

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto shift_fwd(range_type& range, usize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_move_assignable
    {
        _impl_type<range_type>::shift_fwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto shift_bwd(range_type& range, usize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_move_assignable
    {
        _impl_type<range_type>::shift_bwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto shift_by(range_type& range, isize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_swappable
    {
        _impl_type<range_type>::shift_by(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_fwd(range_type& range, usize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_swappable
    {
        _impl_type<range_type>::rotate_fwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_bwd(range_type& range, usize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_swappable
    {
        _impl_type<range_type>::rotate_bwd(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto rotate_by(range_type& range, isize steps) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_swappable
    {
        _impl_type<range_type>::rotate_by(range, steps);
        return range;
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto destroy_elems(range_type& range) -> range_type&
        requires is_mut_range<range_type> and type_info<value_type<range_type>>::is_destructible
    {
        _impl_type<range_type>::destroy_elems(range);
        return range;
    }
}

export namespace atom
{
    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto begin(const range_type& range) -> ranges::iterator_type<range_type>
        requires ranges::is_range<range_type>
    {
        return range.get_iterator();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto end(const range_type& range) -> ranges::iterator_end_type<range_type>
        requires ranges::is_range<range_type>
    {
        return range.get_iterator_end();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto begin(range_type& range) -> ranges::mut_iterator_type<range_type>
        requires ranges::is_mut_range<range_type>
    {
        return range.get_iterator();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type>
    constexpr auto end(range_type& range) -> ranges::mut_iterator_end_type<range_type>
        requires ranges::is_mut_range<range_type>
    {
        return range.get_iterator_end();
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto operator==(const range_type& range, const that_range_type& that_range) -> bool
        requires ranges::is_range<range_type> and ranges::is_range<that_range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<
                             ranges::value_type<that_range_type>>())
    {
        return ranges::_impl_type<range_type>::is_eq(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_range_type>
    constexpr auto operator!=(const range_type& range, const that_range_type& that_range) -> bool
        requires ranges::is_range<range_type> and ranges::is_range<that_range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<
                             ranges::value_type<that_range_type>>())
    {
        return not ranges::_impl_type<range_type>::is_eq(range, that_range);
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type, usize that_count>
    constexpr auto operator==(
        const range_type& range, const that_value_type (&that_arr)[that_count]) -> bool
        requires ranges::is_range<range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<that_value_type>())
    {
        return ranges::_impl_type<range_type>::is_eq(range, ranges::from(that_arr));
    }

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename range_type, typename that_value_type, usize that_count>
    constexpr auto operator!=(
        const range_type& range, const that_value_type (&that_arr)[that_count]) -> bool
        requires ranges::is_range<range_type>
                 and (type_info<ranges::value_type<range_type>>::
                         template is_equality_comparable_with<that_value_type>())
    {
        return not ranges::_impl_type<range_type>::is_eq(range, ranges::from(that_arr));
    }

    template <typename value_type0, typename value_type1>
    constexpr auto operator|(const value_type1& value1, const value_type0& value0) -> decltype(auto)
        requires requires(
            const value_type0& value0, const value_type1& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }

    template <typename value_type0, typename value_type1>
    constexpr auto operator|(value_type1& value1, value_type0& value0) -> decltype(auto)
        requires requires(value_type0& value0, value_type1& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }

    template <typename value_type0, typename value_type1>
    constexpr auto operator|(value_type1&& value1, value_type0&& value0) -> decltype(auto)
        requires requires(value_type0&& value0, value_type1&& value1) { value0.operator|(value1); }
    {
        return value0.operator|(value1);
    }
}
