#pragma once
#include "atom/core/range/fwd_range_extensions.h"
#include "atom/core/range/mut_range_extensions.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_impl_t = void>
    class _mut_fwd_range_extensions_impl
        : public _mut_range_extensions_impl<range_t, _const_range_extensions_impl_t>
    {
        using base_t = _mut_range_extensions_impl<range_t, _const_range_extensions_impl_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t>
    class _mut_fwd_range_extensions_impl<range_t, void>
        : public _mut_fwd_range_extensions_impl<range_t, _fwd_range_extensions_impl<range_t>>
    {
        using base_t = _mut_fwd_range_extensions_impl<range_t, _fwd_range_extensions_impl<range_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename range_t, typename _const_range_extensions_t = void>
    class mut_fwd_range_extensions: public mut_range_extensions<range_t, _const_range_extensions_t>
    {
        using base_t = mut_range_extensions<range_t, _const_range_extensions_t>;

    protected:
        using _impl_t = typename base_t::_impl_t;

    public:
        using value_t = typename base_t::value_t;
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
    class mut_fwd_range_extensions<range_t, void>
        : public mut_fwd_range_extensions<range_t,
              fwd_range_extensions<range_t, _mut_fwd_range_extensions_impl<range_t>>>
    {
        using base_t = mut_fwd_range_extensions<range_t,
            fwd_range_extensions<range_t, _mut_fwd_range_extensions_impl<range_t>>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
