#pragma once
#include "atom/core/core.h"
#include "atom/core/contracts.h"
#include "atom/core/range/array_range_extensions.h"
#include "atom/core/range/mut_jump_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_impl_t = void>
    class _mut_array_range_extensions_impl
        : public _mut_jump_range_extensions_impl<range_t, _const_range_extensions_impl_t>
    {
        using base_t =
            _mut_jump_range_extensions_impl<range_t, _const_range_extensions_impl_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using mut_iter_t = typename base_t::mut_iter_t;
        using mut_iter_end_t = typename base_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        constexpr auto get_mut_data() -> value_t*
        {
            return _range().get_mut_data();
        }

        constexpr auto get_count() const -> usize
        {
            return _range().get_count();
        }

        constexpr auto get_mut_at(usize i) -> value_t&
        {
            return (get_mut_data() + i).get_mut();
        }

        constexpr auto mut_front() -> value_t&
        {
            return get_mut_at(0);
        }

        constexpr auto mut_back() -> value_t&
        {
            ATOM_DEBUG_EXPECTS(get_count() > 0);

            return get_mut_at(get_count() - 1);
        }

        constexpr auto get_mut_iter_at(usize i) -> mut_iter_t
        {
            return base_t::get_mut_iter().next(i);
        }

    protected:
        using base_t::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _mut_array_range_extensions_impl<range_t, void>
        : public _mut_array_range_extensions_impl<range_t,
              _array_range_extensions_impl<range_t>>
    {
        using base_t =
            _mut_array_range_extensions_impl<range_t, _array_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_t = void>
    class mut_array_range_extensions
        : public mut_jump_range_extensions<range_t, _const_range_extensions_t>
    {
        using base_t = mut_jump_range_extensions<range_t, _const_range_extensions_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using mut_iter_t = typename base_t::mut_iter_t;
        using mut_iter_end_t = typename base_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> value_t*
        {
            return _impl().get_mut_data();
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
        constexpr auto get_mut_at(usize i) -> value_t&
        {
            ATOM_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_mut_at(i);
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
        constexpr auto operator[](usize i) -> value_t&
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_mut_at(i);
        }

        using base_t::operator[];

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_front() -> value_t&
        {
            ATOM_DEBUG_EXPECTS(not is_empty(), "range is empty.");

            return _impl().mut_front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_back() -> value_t&
        {
            ATOM_DEBUG_EXPECTS(not is_empty(), "range is empty.");

            return _impl().mut_back();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get iter to element at index `i`.
        ///
        /// # parameters
        ///
        /// - `i`: index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_at(usize i) const -> mut_iter_t
        {
            ATOM_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_mut_iter_at(i);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// view
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range(usize from, usize to) const {}

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range_from(usize from) const {}

        /// ----------------------------------------------------------------------------------------
        /// # to do: review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range_to(usize to) const {}

        using base_t::is_empty;
        using base_t::is_index_in_range;

    protected:
        using base_t::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class mut_array_range_extensions<range_t, void>
        : public mut_array_range_extensions<range_t,
              array_range_extensions<range_t, _mut_array_range_extensions_impl<range_t>>>
    {
        using base_t = mut_array_range_extensions<range_t,
            array_range_extensions<range_t, _mut_array_range_extensions_impl<range_t>>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
