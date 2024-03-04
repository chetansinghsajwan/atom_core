#pragma once
#include "atom/core/range/fwd_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _bidi_range_extensions_impl: public _fwd_range_extensions_impl<range_type>
    {
        using base_type = _fwd_range_extensions_impl<range_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _trange_extensions_impl = void>
    class bidi_range_extensions: public fwd_range_extensions<range_type, _trange_extensions_impl>
    {
        using base_type = fwd_range_extensions<range_type, _trange_extensions_impl>;

    protected:
        using _impl_type = typename base_type::_impl_type;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class bidi_range_extensions<range_type, void>
        : public bidi_range_extensions<range_type, _bidi_range_extensions_impl<range_type>>
    {
        using base_type =
            bidi_range_extensions<range_type, _bidi_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
