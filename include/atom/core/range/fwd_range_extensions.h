#pragma once
#include "atom/core/range/range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _fwd_range_extensions_impl: public _range_extensions_impl<range_t>
    {
        using base_t = _range_extensions_impl<range_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _range_extensions_impl_t = void>
    class fwd_range_extensions: public range_extensions<range_t, _range_extensions_impl_t>
    {
        using base_t = range_extensions<range_t, _range_extensions_impl_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using elem_t = typename base_t::elem_t;
        using iter_t = typename base_t::iter_t;
        using iter_end_t = typename base_t::iter_end_t;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class fwd_range_extensions<range_t, void>
        : public fwd_range_extensions<range_t, _fwd_range_extensions_impl<range_t>>
    {
        using base_t = fwd_range_extensions<range_t, _fwd_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
