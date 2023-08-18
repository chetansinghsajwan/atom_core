#pragma once
#include "RangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// When marked with this flag, mut range implementations doesn't inherit from their range
    /// implementations, this is done to prevent diamond problem.
    /// --------------------------------------------------------------------------------------------
    class _DontInheritRangeImpl {};

    /// --------------------------------------------------------------------------------------------
    /// Requirements for [`MutRangeImpl`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutRangeImpl = false;

    /// --------------------------------------------------------------------------------------------
    /// User defined implementation for [`MutRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutRangeTraitImpl;

    /// --------------------------------------------------------------------------------------------
    /// Implementation for [`MutRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class _MutRangeTraitImpl: public TRangeImpl
    {
        using Base = TRangeImpl;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename TImpl::TMutIter;
        using TMutIterEnd = typename TImpl::TMutIterEnd;

    public:
        constexpr auto mutIter() -> TMutIter
        {
            return _impl.mutIter();
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return _impl.mutIterEnd();
        }

    public:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// Recursive Trait for [`MutRange`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeTrait>
    class MutRangeTraitRecursive: public TRangeTrait
    {
        using Base = TRangeTrait;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename TImpl::TMutIter;
        using TMutIterEnd = typename TImpl::TMutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIter() -> TMutIter
        {
            return _impl.mutIter();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return _impl.mutIterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin()
        {
            return StdIterWrapForAtomIter{ _impl.mutIter() };
        }

        using Base::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end()
        {
            return StdIterWrapForAtomIter{ _impl.mutIterEnd() };
        }

        using Base::end;

    //// -------------------------------------------------------------------------------------------
    ////
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename T1>
        constexpr auto writeElems(T1& val) const -> void
            requires(RAssignable<TElem, T1>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
#pragma optimize("", off)

        template <typename T1>
        constexpr auto writeElemsNoOptimize(T1& val) const -> void
            requires(RAssignable<TElem, T1>)
        {}

#pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shiftFwd(usize steps)
            requires(RMoveAssignable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shiftBwd(usize steps)
            requires(RMoveAssignable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto shiftBy(isize steps)
            requires(RSwappable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotateFwd(usize steps)
            requires(RSwappable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotateBwd(usize steps)
            requires(RSwappable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto rotateBy(isize steps)
            requires(RSwappable<TElem>)
        {}

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroyElems()
            requires(RDestructible<TElem>)
        {}

    public:
        using Base::_impl;
    };
}
