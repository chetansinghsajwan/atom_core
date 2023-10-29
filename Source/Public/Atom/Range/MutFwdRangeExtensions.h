#pragma once
#include "FwdRangeExtensions.h"
#include "MutRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TConstRangeExtensionsImpl = void>
    class _MutFwdRangeExtensionsImpl:
        public _MutRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>
    {
        using Base = _MutRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutFwdRangeExtensionsImpl<TRange, void>:
        public _MutFwdRangeExtensionsImpl<TRange, _FwdRangeExtensionsImpl<TRange>>
    {
        using Base = _MutFwdRangeExtensionsImpl<TRange, _FwdRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TConstRangeExtensions = void>
    class MutFwdRangeExtensions: public MutRangeExtensions<TRange, _TConstRangeExtensions>
    {
        using Base = MutRangeExtensions<TRange, _TConstRangeExtensions>;

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
    class MutFwdRangeExtensions<TRange, void>:
        public MutFwdRangeExtensions<TRange,
            FwdRangeExtensions<TRange, _MutFwdRangeExtensionsImpl<TRange>>>
    {
        using Base = MutFwdRangeExtensions<TRange,
            FwdRangeExtensions<TRange, _MutFwdRangeExtensionsImpl<TRange>>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
