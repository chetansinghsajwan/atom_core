#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <tname TImpl>
    class _DynArrImplHelper: public _ArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        using Base = _ArrImplHelper<TImpl>;

    public:
        using TElem = tname Base::TElem;
        using TIter = tname Base::TIter;
        using TIterEnd = tname Base::TIterEnd;
        using TMutIter = tname Base::TMutIter;
        using TMutIterEnd = tname Base::TMutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <tname T2, tname = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertAt(TIter pos, T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange, tname = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertAt(TIter pos, const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <tname T2, tname = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertFront(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange, tname = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertFront(const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <tname T2, tname = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn InsertBack(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange, tname = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn InsertBack(const TRange& range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <tname T2, tname = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr fn operator +=(T2&& el) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange, tname = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr fn operator +=(const TRange& range) -> TMutIter;

    private:
        // TODO: Refactor this. Should we use T2 here to denote other tname?
        template <tname T2>
        constexpr fn _InsertAt(usize index, T2&& el) -> usize;

        template <tname TIter2>
        constexpr fn _InsertAtCounted(usize index, TIter2 it, usize count) -> usize;

        template <tname TIter2, tname TIterEnd2>
        constexpr fn _InsertAtUncounted(usize index, TIter2 begin, TIterEnd2 end) -> usize;

        template <tname T2>
        constexpr fn _InsertBack(T2&& el) -> usize;

        template <tname TIter2>
        constexpr fn _InsertBackCounted(TIter2 it, usize count) -> usize;

        template <tname TIter2, tname TIterEnd2>
        constexpr fn _InsertBackUncounted(TIter2 begin, TIterEnd2 end) -> usize;

        constexpr fn _ValidateIndexForInsert(isize index) const noex -> bool;

    //// -------------------------------------------------------------------------------------------
    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Removes element at pos{pos}.
        /// 
        /// @PARAM pos: Position to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr fn RemoveAt(TIter pos) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// Removes element at range{range}.
        /// 
        /// @PARAM range: Range to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr fn RemoveRange(Range<TIter, TIterEnd> range) -> TMutIter;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Clear();

    public:
        constexpr fn _RemoveAt(usize index) -> usize;

        constexpr fn _RemoveRange(usize begin, usize count) -> usize;

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Reserve(usize size);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Release();

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn Capacity() const noex -> usize;

        using Base::Iter;
        using Base::IterEnd;
        using Base::MutIter;
        using Base::MutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        constexpr fn _ValidateIter(TIter it) const noex -> bool;
        constexpr fn _UpdateIterDebugId() noex;
        constexpr fn _ValidateIndex(isize index) const noex -> bool;
        constexpr fn _FetchIndex(TIter pos) const noex -> isize;
        constexpr fn _CalcCapGrowth(usize required) const noex -> usize;
        constexpr fn _EnsureCapFor(usize count);

        constexpr fn _ConstructAt(usize index, auto&&... args);
        constexpr fn _DestructAt(usize index);
        constexpr fn _DestructRange(usize index, usize count);
        constexpr fn _MoveRangeFront(usize index, usize count);
        constexpr fn _MoveRangeBack(usize index, usize count);
        constexpr fn _MoveRangeTo(usize index, TElem* dest);
        constexpr fn _RotateRangeBack(usize index, usize count);

        template <tname TRange>
        static constexpr fn _CanGetRangeSize() noex -> bool;

        template <tname TRange>
        static constexpr fn _GetRangeSize(const TRange& range) noex -> usize;

        using Base::_Data;
        using Base::_Count;
        using Base::_Capacity;
        using Base::_AllocMem;
        using Base::_DeallocMem;
    };
}