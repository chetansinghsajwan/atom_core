#pragma once
#include "RangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// User defined implementation for [`MutRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutRangeTraitImpl;

    /// --------------------------------------------------------------------------------------------
    /// Implementation for [`MutRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutRangeTraitImpl:
        private MutRangeTraitImpl<TRange>
    {
    private:
        using _Impl = MutRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = typename _Impl::TIter;
        using TIterEnd = typename _Impl::TIterEnd;
        using TMutIter = typename _Impl::TMutIter;
        using TMutIterEnd = typename _Impl::TMutIterEnd;

    public:
        constexpr auto iter() const -> TIter
        {
            return _Impl::iter();
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

        constexpr auto mutIter() const -> TIter
        {
            return _Impl::mutIter();
        }

        constexpr auto mutIterEnd() const -> TIterEnd
        {
            return _Impl::mutIterEnd();
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Trait for [`MutRange`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutRangeTrait:
        public RangeTrait<TRange>
    {
        using Base = RangeTrait<TRange>;
        using _Impl = _MutRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = typename _Impl::TIter;
        using TIterEnd = typename _Impl::TIterEnd;
        using TMutIter = typename _Impl::TMutIter;
        using TMutIterEnd = typename _Impl::TMutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() -> TMutIter
        {
            return _Impl::mutIter();
        }

        using Base::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() -> TMutIterEnd
        {
            return _Impl::mutIterEnd();
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
    };
}
