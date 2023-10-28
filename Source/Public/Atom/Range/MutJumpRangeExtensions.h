#pragma once
#include "JumpRangeExtensions.h"
#include "MutBidiRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl, class _TRangeExtensionsImpl = void>
    class _MutJumpRangeExtensionsImpl: 
        public _MutBidiRangeExtensionsImpl<TRangeImpl, _TRangeExtensionsImpl>
    {
        using Base = _MutBidiRangeExtensionsImpl<TRangeImpl, _TRangeExtensionsImpl>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class _MutJumpRangeExtensionsImpl<TRangeImpl, void>: 
        public _MutBidiRangeExtensionsImpl<TRangeImpl, _BidiRangeExtensionsImpl<TRangeImpl>>
    {
        using Base = _MutBidiRangeExtensionsImpl<TRangeImpl, _BidiRangeExtensionsImpl<TRangeImpl>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensions = void>
    class MutJumpRangeExtensions: public MutBidiRangeExtensions<TRange, _TRangeExtensions>
    {
        using Base = MutBidiRangeExtensions<TRange, _TRangeExtensions>;

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
