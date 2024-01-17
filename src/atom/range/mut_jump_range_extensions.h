#pragma once
#include "jump_range_extensions.h"
#include "mut_bidi_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, class _tconst_range_extensions_impl = void>
    class _mut_jump_range_extensions_impl
        : public _mut_bidi_range_extensions_impl<range_type, _tconst_range_extensions_impl>
    {
        using base_type = _mut_bidi_range_extensions_impl<range_type, _tconst_range_extensions_impl>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class _mut_jump_range_extensions_impl<range_type, void>
        : public _mut_bidi_range_extensions_impl<range_type, _bidi_range_extensions_impl<range_type>>
    {
        using base_type = _mut_bidi_range_extensions_impl<range_type, _bidi_range_extensions_impl<range_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type, typename _tconst_range_extensions = void>
    class mut_jump_range_extensions: public mut_bidi_range_extensions<range_type, _tconst_range_extensions>
    {
        using base_type = mut_bidi_range_extensions<range_type, _tconst_range_extensions>;

    protected:
        using _timpl = typename base_type::_timpl;

    public:
        using elem_type = typename base_type::elem_type;
        using iter_type = typename base_type::iter_type;
        using iter_end_type = typename base_type::iter_end_type;
        using mut_iter_type = typename base_type::mut_iter_type;
        using mut_iter_end_type = typename base_type::mut_iter_end_type;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
    class mut_jump_range_extensions<range_type, void>
        : public mut_jump_range_extensions<range_type,
              jump_range_extensions<range_type, _mut_jump_range_extensions_impl<range_type>>>
    {
        using base_type = mut_jump_range_extensions<range_type,
            jump_range_extensions<range_type, _mut_jump_range_extensions_impl<range_type>>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
