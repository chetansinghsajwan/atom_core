#pragma once
#include "RangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class FwdRangeTraitRecursive: public RangeTraitRecursive<TRangeImpl>
    {
        using Base = RangeTraitRecursive<TRangeImpl>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class BidiRangeTraitRecursive: public FwdRangeTraitRecursive<TRangeImpl>
    {
        using Base = FwdRangeTraitRecursive<TRangeImpl>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class JumpRangeTraitRecursive: public BidiRangeTraitRecursive<TRangeImpl>
    {
        using Base = BidiRangeTraitRecursive<TRangeImpl>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
    };
}