#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename TImpl>
    class _DynArrImplHelper: public _ArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        using Base = _ArrImplHelper<TImpl>;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

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
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr TMutIter InsertAt(TIter pos, T2&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr TMutIter InsertAt(TIter pos, const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr TMutIter InsertFront(T2&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr TMutIter InsertFront(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr TMutIter InsertBack(T2&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr TMutIter InsertBack(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <typename T2, typename = TTI::TEnableIf<
            RSameAsUnqualified<T2, TElem>>>
        constexpr TMutIter operator +=(T2&& el);

        /// ----------------------------------------------------------------------------------------
        /// Calls {InsertBack(...)}.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename = TTI::TEnableIf<
            RRangeOf<TRange, TElem>>>
        constexpr TMutIter operator +=(const TRange& range);

    private:
        // TODO: Refactor this. Should we use T2 here to denote other typename?
        template <typename T2>
        constexpr usize _InsertAt(usize index, T2&& el);

        template <typename TIter2>
        constexpr usize _InsertAtCounted(usize index, TIter2 it, usize count);

        template <typename TIter2, typename TIterEnd2>
        constexpr usize _InsertAtUncounted(usize index, TIter2 begin, TIterEnd2 end);

        template <typename T2>
        constexpr usize _InsertBack(T2&& el);

        template <typename TIter2>
        constexpr usize _InsertBackCounted(TIter2 it, usize count);

        template <typename TIter2, typename TIterEnd2>
        constexpr usize _InsertBackUncounted(TIter2 begin, TIterEnd2 end);

        constexpr bool _ValidateIndexForInsert(isize index) const noexcept;

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
        constexpr TMutIter RemoveAt(TIter pos);

        /// ----------------------------------------------------------------------------------------
        /// Removes element at range{range}.
        /// 
        /// @PARAM range: Range to remove element at.
        /// 
        /// @RETURNS {TMutIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter RemoveRange(Range<TIter, TIterEnd> range);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr void Clear();

    public:
        constexpr usize _RemoveAt(usize index);

        constexpr usize _RemoveRange(usize begin, usize count);

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr void Reserve(usize size);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr void Release();

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr usize Capacity() const noexcept;

        using Base::Iter;
        using Base::IterEnd;
        using Base::MutIter;
        using Base::MutIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        constexpr bool _ValidateIter(TIter it) const noexcept;
        constexpr void _UpdateIterDebugId() noexcept;
        constexpr bool _ValidateIndex(isize index) const noexcept;
        constexpr isize _FetchIndex(TIter pos) const noexcept;
        constexpr usize _CalcCapGrowth(usize required) const noexcept;
        constexpr void _EnsureCapFor(usize count);

        constexpr void _ConstructAt(usize index, auto&&... args);
        constexpr void _DestructAt(usize index);
        constexpr void _DestructRange(usize index, usize count);
        constexpr void _MoveRangeFront(usize index, usize count);
        constexpr void _MoveRangeBack(usize index, usize count);
        constexpr void _MoveRangeTo(usize index, TElem* dest);
        constexpr void _RotateRangeBack(usize index, usize count);

        template <typename TRange>
        static constexpr bool _CanGetRangeSize() noexcept;

        template <typename TRange>
        static constexpr usize _GetRangeSize(const TRange& range) noexcept;

        using Base::_Data;
        using Base::_Count;
        using Base::_Capacity;
        using Base::_AllocMem;
        using Base::_DeallocMem;
    };
}