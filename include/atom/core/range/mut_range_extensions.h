#pragma once
#include "atom/core/core.h"
#include "atom/core/core/requirements.h"
#include "atom/core/preprocessors.h"
#include "atom/core/range/range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// implementation for [`mut_range_extensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_extensions_impl_t = void>
    class _mut_range_extensions_impl: public _const_extensions_impl_t
    {
        using base_t = _const_extensions_impl_t;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using mut_iter_t = typename _impl_t::mut_iter_t;
        using mut_iter_end_t = typename _impl_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return _range().get_mut_iter();
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_t
        {
            return _range().get_mut_iter_end();
        }

    protected:
        using base_t::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _mut_range_extensions_impl<range_t, void>
        : public _mut_range_extensions_impl<range_t, _range_extensions_impl<range_t>>
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_t = void>
    class mut_range_extensions: public _const_range_extensions_t
    {
        using base_t = _const_range_extensions_t;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using mut_iter_t = typename _impl_t::mut_iter_t;
        using mut_iter_end_t = typename _impl_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter() -> mut_iter_t
        {
            return _impl().get_mut_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_end() -> mut_iter_end_t
        {
            return _impl().get_mut_iter_end();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin()
        {
            return std_iter_wrap_for_atom_iter(_impl().get_mut_iter());
        }

        using base_t::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end()
        {
            return std_iter_wrap_for_atom_iter(_impl().get_mut_iter_end());
        }

        using base_t::end;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// write
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type1>
        constexpr auto write_elems(type1& val) const -> void
            requires(is_assignable<value_t, type1>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <typename type1>
        constexpr auto write_elems_no_optimize(type1& val) const -> void
            requires(is_assignable<value_t, type1>)
        {}

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_fwd(usize steps)
            requires(is_move_assignable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_bwd(usize steps)
            requires(is_move_assignable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by(isize steps)
            requires(is_swappable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_fwd(usize steps)
            requires(is_swappable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_bwd(usize steps)
            requires(is_swappable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_by(isize steps)
            requires(is_swappable<value_t>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy_elems()
            requires(is_destructible<value_t>)
        {}

    protected:
        using base_t::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class mut_range_extensions<range_t, void>
        : public mut_range_extensions<range_t,
              range_extensions<range_t, _mut_range_extensions_impl<range_t>>>
    {
        using base_t = mut_range_extensions<range_t,
            range_extensions<range_t, _mut_range_extensions_impl<range_t>>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
