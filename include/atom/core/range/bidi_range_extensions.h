#pragma once
#include "atom/core/range/fwd_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _bidi_range_extensions_impl: public _fwd_range_extensions_impl<range_t>
    {
        using base_t = _fwd_range_extensions_impl<range_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _range_extensions_impl_t = void>
    class bidi_range_extensions: public fwd_range_extensions<range_t, _range_extensions_impl_t>
    {
        using base_t = fwd_range_extensions<range_t, _range_extensions_impl_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
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
    class bidi_range_extensions<range_t, void>
        : public bidi_range_extensions<range_t, _bidi_range_extensions_impl<range_t>>
    {
        using base_t =
            bidi_range_extensions<range_t, _bidi_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
