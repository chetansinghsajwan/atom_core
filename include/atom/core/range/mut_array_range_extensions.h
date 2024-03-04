#pragma once
#include "atom/core/core.h"
#include "atom/core/contracts_decl.h"
#include "atom/core/range/array_range_extensions.h"
#include "atom/core/range/mut_jump_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions_impl = void>
    class _mut_array_range_extensions_impl
        : public _mut_jump_range_extensions_impl<range_type, _tconst_range_extensions_impl>
    {
        using base_type =
            _mut_jump_range_extensions_impl<range_type, _tconst_range_extensions_impl>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename base_type::mut_iter_type;
        using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        constexpr auto get_mut_data() -> elem_type*
        {
            return _range().get_mut_data();
        }

        constexpr auto get_count() const -> usize
        {
            return _range().get_count();
        }

        constexpr auto get_mut_at(usize i) -> elem_type&
        {
            return (get_mut_data() + i).get_mut();
        }

        constexpr auto mut_front() -> elem_type&
        {
            return get_mut_at(0);
        }

        constexpr auto mut_back() -> elem_type&
        {
            contracts::debug_expects(get_count() > 0);

            return get_mut_at(get_count() - 1);
        }

        constexpr auto get_mut_iter_at(usize i) -> mut_iter_type
        {
            return base_type::get_mut_iter().next(i);
        }

    protected:
        using base_type::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _mut_array_range_extensions_impl<range_type, void>
        : public _mut_array_range_extensions_impl<range_type,
              _array_range_extensions_impl<range_type>>
    {
        using base_type =
            _mut_array_range_extensions_impl<range_type, _array_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions = void>
    class mut_array_range_extensions
        : public mut_jump_range_extensions<range_type, _tconst_range_extensions>
    {
        using base_type = mut_jump_range_extensions<range_type, _tconst_range_extensions>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename base_type::mut_iter_type;
        using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_data() -> elem_type*
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
        constexpr auto get_mut_at(usize i) -> elem_type&
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

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
        constexpr auto operator[](usize i) -> elem_type&
        {
            contracts::debug_expects(is_index_in_range(i), "index is out of range.");

            return _impl().get_mut_at(i);
        }

        using base_type::operator[];

        /// ----------------------------------------------------------------------------------------
        /// access first element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_front() -> elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

            return _impl().mut_front();
        }

        /// ----------------------------------------------------------------------------------------
        /// access last element.
        ///
        /// # time complexity
        /// constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_back() -> elem_type&
        {
            contracts::debug_expects(not is_empty(), "range is empty.");

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
        constexpr auto get_mut_iter_at(usize i) const -> mut_iter_type
        {
            contracts::expects(is_index_in_range(i), "index is out of range.");

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

        using base_type::is_empty;
        using base_type::is_index_in_range;

    protected:
        using base_type::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class mut_array_range_extensions<range_type, void>
        : public mut_array_range_extensions<range_type,
              array_range_extensions<range_type, _mut_array_range_extensions_impl<range_type>>>
    {
        using base_type = mut_array_range_extensions<range_type,
            array_range_extensions<range_type, _mut_array_range_extensions_impl<range_type>>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
