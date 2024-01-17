#pragma once
#include "RangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _FwdRangeExtensionsImpl: public _RangeExtensionsImpl<TRange>
    {
        using Base = _RangeExtensionsImpl<TRange>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensionsImpl = void>
    class FwdRangeExtensions: public RangeExtensions<TRange, _TRangeExtensionsImpl>
    {
        using Base = RangeExtensions<TRange, _TRangeExtensionsImpl>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class FwdRangeExtensions<TRange, void>
        : public FwdRangeExtensions<TRange, _FwdRangeExtensionsImpl<TRange>>
    {
        using Base = FwdRangeExtensions<TRange, _FwdRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
