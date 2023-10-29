#pragma once
#include "BidiRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _JumpRangeExtensionsImpl: public _BidiRangeExtensionsImpl<TRange>
    {
        using Base = _BidiRangeExtensionsImpl<TRange>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensionsImpl = void>
    class JumpRangeExtensions: public BidiRangeExtensions<TRange, _TRangeExtensionsImpl>
    {
        using Base = BidiRangeExtensions<TRange, _TRangeExtensionsImpl>;

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
    class JumpRangeExtensions<TRange, void>:
        public JumpRangeExtensions<TRange, _JumpRangeExtensionsImpl<TRange>>
    {
        using Base = JumpRangeExtensions<TRange, _JumpRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
