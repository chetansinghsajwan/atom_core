#pragma once
#include "atom/core/range/jump_range_extensions.h"
#include "atom/core/range/mut_bidi_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, class _tconst_range_extensions_impl = void>
    class _mut_jump_range_extensions_impl
        : public _mut_bidi_range_extensions_impl<range_t, _tconst_range_extensions_impl>
    {
        using base_t =
            _mut_bidi_range_extensions_impl<range_t, _tconst_range_extensions_impl>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _mut_jump_range_extensions_impl<range_t, void>
        : public _mut_bidi_range_extensions_impl<range_t,
              _bidi_range_extensions_impl<range_t>>
    {
        using base_t =
            _mut_bidi_range_extensions_impl<range_t, _bidi_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_t = void>
    class mut_jump_range_extensions
        : public mut_bidi_range_extensions<range_t, _const_range_extensions_t>
    {
        using base_t = mut_bidi_range_extensions<range_t, _const_range_extensions_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using elem_t = typename base_t::elem_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;
        using mut_iter_t = typename base_t::mut_iter_t;
        using mut_iter_end_t = typename base_t::mut_iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class mut_jump_range_extensions<range_t, void>
        : public mut_jump_range_extensions<range_t,
              jump_range_extensions<range_t, _mut_jump_range_extensions_impl<range_t>>>
    {
        using base_t = mut_jump_range_extensions<range_t,
            jump_range_extensions<range_t, _mut_jump_range_extensions_impl<range_t>>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
