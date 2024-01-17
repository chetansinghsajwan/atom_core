#pragma once
#include "FwdRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _BidiRangeExtensionsImpl: public _FwdRangeExtensionsImpl<TRange>
    {
        using Base = _FwdRangeExtensionsImpl<TRange>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensionsImpl = void>
    class BidiRangeExtensions: public FwdRangeExtensions<TRange, _TRangeExtensionsImpl>
    {
        using Base = FwdRangeExtensions<TRange, _TRangeExtensionsImpl>;

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
    class BidiRangeExtensions<TRange, void>
        : public BidiRangeExtensions<TRange, _BidiRangeExtensionsImpl<TRange>>
    {
        using Base = BidiRangeExtensions<TRange, _BidiRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
