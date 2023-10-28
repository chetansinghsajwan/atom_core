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
    template <typename TRange, typename _TImpl_ = void>
    class JumpRangeExtensions: public BidiRangeExtensions<TRange, _TImpl_>
    {
        using Base = BidiRangeExtensions<TRange, _TImpl_>;

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
