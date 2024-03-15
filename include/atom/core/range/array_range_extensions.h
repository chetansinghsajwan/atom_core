#pragma once
#include "atom/core/core.h"
#include "atom/core/contracts.h"
#include "atom/core/range/jump_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implementation of [`array_range_extensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _array_range_extensions_impl: public _jump_range_extensions_impl<range_t>
    {
        using base_t = _jump_range_extensions_impl<range_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        constexpr auto get_data() const -> const value_t*
        {
            return _range().get_data();
        }

        constexpr auto get_count() const -> usize
        {
            return _range().get_count();
        }

        constexpr auto get_at(usize i) const -> const value_t&
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(i));

            return get_data()[i];
        }

        constexpr auto get_front() const -> const value_t&
        {
            return get_at(0);
        }

        constexpr auto get_back() const -> const value_t&
        {
            return get_back(get_count() - 1);
        }

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return i < get_count();
        }

        constexpr auto get_iter_at(usize i) const -> iter_t
        {
            return base_t::get_iter().next(i);
        }

    protected:
        using base_t::_range;
    };

    /// --------------------------------------------------------------------------------------------
    /// [array_range_extensions].
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _range_extensions_impl_t = void>
    class array_range_extensions: public jump_range_extensions<range_t, _range_extensions_impl_t>
    {
        using base_t = jump_range_extensions<range_t, _range_extensions_impl_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using view_t = int;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const value_t*
        {
            return _impl().get_data();
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
        constexpr auto get_at(usize i) const -> const value_t&
        {
            ATOM_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_at(i);
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
        constexpr auto operator[](usize i) const -> const value_t&
        {
            ATOM_DEBUG_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_front() const -> const value_t&
        {
            ATOM_DEBUG_EXPECTS(not is_empty(), "range is empty.");

            return _impl().get_front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_back() const -> const value_t&
        {
            ATOM_DEBUG_EXPECTS(not is_empty(), "range is empty.");

            return _impl().get_back();
        }

        /// ----------------------------------------------------------------------------------------
        /// get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_count() const -> usize
        {
            return _impl().get_count();
        }

        /// ----------------------------------------------------------------------------------------
        /// is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_empty() const -> bool
        {
            return _impl().get_count() == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get iter to element at index `i`.
        ///
        /// # parameters
        ///
        /// - `i`: index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_iter_at(usize i) const -> iter_t
        {
            ATOM_EXPECTS(is_index_in_range(i), "index is out of range.");

            return _impl().get_iter_at(i);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// view
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_view(usize from, usize to) const -> view_t
        {
            return view_t();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_view_from(usize from) const -> view_t
        {
            return view_t();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_view_to(usize to) const -> view_t
        {
            return view_t();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// validation
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return _impl().is_index_in_range(i);
        }

    protected:
        using base_t::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class array_range_extensions<range_t, void>
        : public array_range_extensions<range_t, _array_range_extensions_impl<range_t>>
    {
        using base_t = array_range_extensions<range_t, _array_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
