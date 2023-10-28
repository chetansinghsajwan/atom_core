#pragma once
#include "RangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Implementation for [`MutRangeExtensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TExtensionsImpl = void>
    class _MutRangeExtensionsImpl: public _TExtensionsImpl
    {
        using Base = _TExtensionsImpl;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename _TImpl::TMutIter;
        using TMutIterEnd = typename _TImpl::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        constexpr auto mutIter() -> TMutIter
        {
            return _impl().mutIter();
        }

        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return _impl().mutIterEnd();
        }

    protected:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutRangeExtensionsImpl<TRange, void>:
        public _MutRangeExtensionsImpl<TRange, _RangeExtensionsImpl<TRange>> {};

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensions = void>
    class MutRangeExtensions: public _TRangeExtensions
    {
        using Base = _TRangeExtensions;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename _TImpl::TMutIter;
        using TMutIterEnd = typename _TImpl::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIter() -> TMutIter
        {
            return _impl().mutIter();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIterEnd() -> TMutIterEnd
        {
            return _impl().mutIterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin()
        {
            return StdIterWrapForAtomIter{ _impl().mutIter() };
        }

        using Base::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end()
        {
            return StdIterWrapForAtomIter{ _impl().mutIterEnd() };
        }

        using Base::end;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Write
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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
ATOM_PRAGMA_OPTIMIZE_OFF

        template <typename T1>
        constexpr auto writeElemsNoOptimize(T1& val) const -> void
            requires(RAssignable<TElem, T1>)
        {}

ATOM_PRAGMA_OPTIMIZE_ON

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

    protected:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutRangeExtensions<TRange, void>:
        public MutRangeExtensions<TRange, RangeExtensions<TRange, _MutRangeExtensionsImpl<TRange>>>
    {
        using Base = MutRangeExtensions<TRange, 
            RangeExtensions<TRange, _MutRangeExtensionsImpl<TRange>>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
