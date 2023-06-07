#pragma once
#include "_ArrImplHelper.decl.h"
#include "Atom/Memory/ObjHelper.h"

namespace Atom
{
    template <typename TImpl>
    class _DynArrImplHelper: pub _ArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        prim using _TBase = _ArrImplHelper<TImpl>;
        pubm using TElem = typename _TBase::TElem;
        pubm using TIter = typename _TBase::TIter;
        pubm using TIterEnd = typename _TBase::TIterEnd;
        pubm using TMutIter = typename _TBase::TMutIter;
        pubm using TMutIterEnd = typename _TBase::TMutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename U, typename = TTI::TEnableIf<
            RSameAsUnqualified<U, TElem>>>
        cexpr TMutIter InsertAt(TIter pos, U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange, typename = TTI::TEnableIf<
            RRange<TRange, TElem>>>
        cexpr TMutIter InsertAt(TIter pos, const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename U, typename = TTI::TEnableIf<
            RSameAsUnqualified<U, TElem>>>
        cexpr TMutIter InsertFront(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange, typename = TTI::TEnableIf<
            RRange<TRange, TElem>>>
        cexpr TMutIter InsertFront(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename U, typename = TTI::TEnableIf<
            RSameAsUnqualified<U, TElem>>>
        cexpr TMutIter InsertBack(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange, typename = TTI::TEnableIf<
            RRange<TRange, TElem>>>
        cexpr TMutIter InsertBack(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename U, typename = TTI::TEnableIf<
            RSameAsUnqualified<U, TElem>>>
        cexpr TMutIter operator +=(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        pubm template <typename TRange, typename = TTI::TEnableIf<
            RRange<TRange, TElem>>>
        cexpr TMutIter operator +=(const TRange& range);

        // TODO: Refactor this. Should we use U here to denote other typename?
        prim template <typename U>
        cexpr usize _InsertAt(usize index, U&& el);

        prim template <typename UIter>
        cexpr usize _InsertAtCounted(usize index, UIter it, usize count);

        prim template <typename UIter, typename UIterEnd>
        cexpr usize _InsertAtUncounted(usize index, UIter begin, UIterEnd end);

        prim template <typename U>
        cexpr usize _InsertBack(U&& el);

        prim template <typename UIter>
        cexpr usize _InsertBackCounted(UIter it, usize count);

        prim template <typename UIter, typename UIterEnd>
        cexpr usize _InsertBackUncounted(UIter begin, UIterEnd end);

        prim cexpr bool _ValidateIndexForInsert(isize index) const noex;

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// Removes element at pos{pos}.
        /// 
        /// @PARAM pos: Position to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr TMutIter RemoveAt(TIter pos);

        /// ----------------------------------------------------------------------------------------
        /// Removes element at range{range}.
        /// 
        /// @PARAM range: Range to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        pubm cexpr TMutIter RemoveRange(Range<TIter, TIterEnd> range);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        cexpr void Clear();

        prim cexpr usize _RemoveAt(usize index);

        prim cexpr usize _RemoveRange(usize begin, usize count);

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pubm cexpr void Reserve(usize size);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pubm cexpr void Release();

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pubm cexpr usize Capacity() const noex;

        pubm using _TBase::Iter;
        pubm using _TBase::IterEnd;
        pubm using _TBase::MutIter;
        pubm using _TBase::MutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

        prom cexpr bool _ValidateIter(TIter it) const noex;
        prom cexpr void _UpdateIterDebugId() noex;
        prom cexpr bool _ValidateIndex(isize index) const noex;
        prom cexpr isize _FetchIndex(TIter pos) const noex;
        prom cexpr usize _CalcCapGrowth(usize required) const noex;
        prom cexpr void _EnsureCapFor(usize count);

        prom cexpr void _ConstructAt(usize index, auto&&... args);
        prom cexpr void _DestructAt(usize index);
        prom cexpr void _DestructRange(usize index, usize count);
        prom cexpr void _MoveRangeFront(usize index, usize count);
        prom cexpr void _MoveRangeBack(usize index, usize count);
        prom cexpr void _MoveRangeTo(usize index, TElem* dest);
        prom cexpr void _RotateRangeBack(usize index, usize count);

        prom template <typename TRange>
        static cexpr bool _CanGetRangeSize() noex;

        prom template <typename TRange>
        static cexpr usize _GetRangeSize(const TRange& range) noex;

        prom using _TBase::_Data;
        prom using _TBase::_Count;
        prom using _TBase::_Capacity;
        prom using _TBase::_AllocMem;
        prom using _TBase::_DeallocMem;

        prim static cexpr ObjHelper<TElem> _objHelper;
    };
}