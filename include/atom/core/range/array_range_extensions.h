#pragma once
#include "atom/core/core.h"
#include "atom/core/contracts_decl.h"
#include "atom/core/range/jump_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implementation of [`array_range_extensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _array_range_extensions_impl: public _jump_range_extensions_impl<range_type>
    {
        using base_type = _jump_range_extensions_impl<range_type>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        constexpr auto get_data() const -> const elem_type*
        {
            return _range().get_data();
        }

        constexpr auto get_count() const -> usize
        {
            return _range().get_count();
        }

        constexpr auto get_at(usize i) const -> const elem_type&
        {
            contracts::debug_expects(is_index_in_range(i));

            return get_data()[i];
        }

        constexpr auto get_front() const -> const elem_type&
        {
            return get_at(0);
        }

        constexpr auto get_back() const -> const elem_type&
        {
            return get_back(get_count() - 1);
        }

        constexpr auto is_index_in_range(usize i) const -> bool
        {
            return i < get_count();
        }

        constexpr auto get_iter_at(usize i) const -> iter_type
        {
            return base_type::get_iter().next(i);
        }

    protected:
        using base_type::_range;
    };

    /// --------------------------------------------------------------------------------------------
    /// [array_range_extensions].
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _trange_extensions_impl_ = void>
    class array_range_extensions: public jump_range_extensions<range_type, _trange_extensions_impl_>
    {
        using base_type = jump_range_extensions<range_type, _trange_extensions_impl_>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using view_type = int;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_data() const -> const elem_type*
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
        constexpr auto get_at(usize i) const -> const elem_type&
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

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
        constexpr auto operator[](usize i) const -> const elem_type&
        {
            contracts::debug_expects(is_index_in_range(i), "index is out of range.");

            return _impl().get_at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_front() const -> const elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().get_front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_back() const -> const elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

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
        constexpr auto get_iter_at(usize i) const -> iter_type
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

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
        constexpr auto get_view(usize from, usize to) const -> view_type
        {
            return view_type();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_view_from(usize from) const -> view_type
        {
            return view_type();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_view_to(usize to) const -> view_type
        {
            return view_type();
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
        using base_type::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class array_range_extensions<range_type, void>
        : public array_range_extensions<range_type, _array_range_extensions_impl<range_type>>
    {
        using base_type =
            array_range_extensions<range_type, _array_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
