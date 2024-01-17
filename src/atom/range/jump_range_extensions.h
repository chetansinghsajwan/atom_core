#pragma once
#include "bidi_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _jump_range_extensions_impl: public _bidi_range_extensions_impl<range_type>
    {
        using base_type = _bidi_range_extensions_impl<range_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _trange_extensions_impl = void>
    class jump_range_extensions: public bidi_range_extensions<range_type, _trange_extensions_impl>
    {
        using base_type = bidi_range_extensions<range_type, _trange_extensions_impl>;

    protected:
        using _timpl = typename base_type::_timpl;

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
    class jump_range_extensions<range_type, void>
        : public jump_range_extensions<range_type, _jump_range_extensions_impl<range_type>>
    {
        using base_type = jump_range_extensions<range_type, _jump_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
