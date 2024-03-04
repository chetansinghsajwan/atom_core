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
    template <typename range_type, typename _tconst_extensions_impl = void>
    class _mut_range_extensions_impl: public _tconst_extensions_impl
    {
        using base_type = _tconst_extensions_impl;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename _impl_type::mut_iter_type;
        using mut_iter_end_type = typename _impl_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return _range().get_mut_iter();
        }

        constexpr auto get_mut_iter_end() -> mut_iter_end_type
        {
            return _range().get_mut_iter_end();
        }

    protected:
        using base_type::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _mut_range_extensions_impl<range_type, void>
        : public _mut_range_extensions_impl<range_type, _range_extensions_impl<range_type>>
    {};

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions = void>
    class mut_range_extensions: public _tconst_range_extensions
    {
        using base_type = _tconst_range_extensions;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename _impl_type::mut_iter_type;
        using mut_iter_end_type = typename _impl_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter() -> mut_iter_type
        {
            return _impl().get_mut_iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_iter_end() -> mut_iter_end_type
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

        using base_type::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end()
        {
            return std_iter_wrap_for_atom_iter(_impl().get_mut_iter_end());
        }

        using base_type::end;

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
            requires(rassignable<elem_type, type1>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        ATOM_PRAGMA_OPTIMIZE_OFF

        template <typename type1>
        constexpr auto write_elems_no_optimize(type1& val) const -> void
            requires(rassignable<elem_type, type1>)
        {}

        ATOM_PRAGMA_OPTIMIZE_ON

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_fwd(usize steps)
            requires(rmove_assignable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_bwd(usize steps)
            requires(rmove_assignable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shift_by(isize steps)
            requires(rswappable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_fwd(usize steps)
            requires(rswappable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_bwd(usize steps)
            requires(rswappable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotate_by(isize steps)
            requires(rswappable<elem_type>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy_elems()
            requires(rdestructible<elem_type>)
        {}

    protected:
        using base_type::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class mut_range_extensions<range_type, void>
        : public mut_range_extensions<range_type,
              range_extensions<range_type, _mut_range_extensions_impl<range_type>>>
    {
        using base_type = mut_range_extensions<range_type,
            range_extensions<range_type, _mut_range_extensions_impl<range_type>>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
