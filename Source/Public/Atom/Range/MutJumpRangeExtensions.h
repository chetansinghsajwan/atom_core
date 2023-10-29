#pragma once
#include "JumpRangeExtensions.h"
#include "MutBidiRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, class _TConstRangeExtensionsImpl = void>
    class _MutJumpRangeExtensionsImpl:
        public _MutBidiRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>
    {
        using Base = _MutBidiRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutJumpRangeExtensionsImpl<TRange, void>:
        public _MutBidiRangeExtensionsImpl<TRange, _BidiRangeExtensionsImpl<TRange>>
    {
        using Base = _MutBidiRangeExtensionsImpl<TRange, _BidiRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TConstRangeExtensions = void>
    class MutJumpRangeExtensions: public MutBidiRangeExtensions<TRange, _TConstRangeExtensions>
    {
        using Base = MutBidiRangeExtensions<TRange, _TConstRangeExtensions>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutJumpRangeExtensions<TRange, void>:
        public MutJumpRangeExtensions<TRange,
            JumpRangeExtensions<TRange, _MutJumpRangeExtensionsImpl<TRange>>>
    {
        using Base = MutJumpRangeExtensions<TRange,
            JumpRangeExtensions<TRange, _MutJumpRangeExtensionsImpl<TRange>>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
