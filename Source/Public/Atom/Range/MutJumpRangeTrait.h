#pragma once
#include "MutRangeTrait.h"
#include "JumpRangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeTrait>
    class MutFwdRangeTraitRecursive: public MutRangeTraitRecursive<TRangeTrait>
    {
        using Base = MutRangeTraitRecursive<TRangeTrait>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeTrait>
    class MutBidiRangeTraitRecursive: public MutFwdRangeTraitRecursive<TRangeTrait>
    {
        using Base = MutFwdRangeTraitRecursive<TRangeTrait>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeTrait>
    class MutJumpRangeTraitRecursive: public MutBidiRangeTraitRecursive<TRangeTrait>
    {
        using Base = MutBidiRangeTraitRecursive<TRangeTrait>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;
    };
}