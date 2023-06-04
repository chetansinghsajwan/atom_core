#pragma once
#include "RangeReq.h"
#include "OutputReq.h"
#include "InsertableReq.h"
#include "Range.h"
#include "ArrayIterator.h"
#include "Atom/Invokable/Invokable.h"
#include "Atom/Memory/ObjHelper.h"
#include "Atom/Memory/DefaultMemAllocator.h"

namespace Atom
{
    template <typename T, typename TMemAlloc = DefaultMemAllocator>
    class DynamicArray
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

    public:
        using TElem = T;
        using TIter = ArrayIterator<const T>;
        using TIterEnd = TIter;
        using TMutIter = ArrayIterator<T>;
        using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Constructors, Operators and Destructor
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray() noexcept;

        /// ----------------------------------------------------------------------------------------
        /// CopyConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const DynamicArray& in);

        /// ----------------------------------------------------------------------------------------
        /// MoveConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(DynamicArray&& in) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// ParameterizedConstructor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr DynamicArray(const TRange& it);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter operator +=(const T& in);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter operator +=(T&& in);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr TMutIter operator +=(const TRange& it);

        /// ----------------------------------------------------------------------------------------
        /// Destructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynamicArray();

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr T* Data() noexcept;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T* Data() const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// Count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr usize Count() const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// Count of elements we have space for.
        /// ----------------------------------------------------------------------------------------
        constexpr usize Capacity() const noexcept;

        /// ----------------------------------------------------------------------------------------
        /// Is array empty.
        /// ----------------------------------------------------------------------------------------
        constexpr bool IsEmpty() const noexcept;

    //// -------------------------------------------------------------------------------------------
    //// Iter
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// {TIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter Iter() const noexcept
        {
            return TIter{ _arr };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter IterEnd() const noexcept
        {
            return TIter{ _arr + _count };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter MutIter() noexcept
        {
            return TMutIter{ _arr };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIterEnd} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIterEnd MutIterEnd() noexcept
        {
            return TMutIterEnd{ _arr + _count + 1};
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter begin() const noexcept { return Iter(); }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter end() const noexcept { return IterEnd(); }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter begin() noexcept { return MutIter(); }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TMutIter end() noexcept { return MutIterEnd(); }

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
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TMutIter InsertAt(TMutIter pos, U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr TMutIter InsertAt(TMutIter pos, const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TMutIter InsertFront(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr TMutIter InsertFront(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TMutIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TMutIter InsertBack(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TMutIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr TMutIter InsertBack(const TRange& range);

    private:
        template <typename U>
        constexpr usize _InsertAt(usize index, U&& el);

        template <typename TOtherIter>
        constexpr usize _InsertAtCounted(usize index, TOtherIter it, usize count);

        template <typename TOtherIter, typename TOtherIterEnd>
        constexpr usize _InsertAtUncounted(usize index, TOtherIter begin, TOtherIterEnd end);

        template <typename U>
        constexpr usize _InsertBack(U&& el);

        template <typename TOtherIter>
        constexpr usize _InsertBackCounted(TOtherIter it, usize count);

        template <typename TOtherIter, typename TOtherIterEnd>
        constexpr usize _InsertBackUncounted(TOtherIter begin, TOtherIterEnd end);

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

    private:
        constexpr usize _RemoveAt(usize index);

        constexpr usize _RemoveRange(usize begin, usize count);

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, T>
        constexpr usize Remove(const TRange& it);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<bool(const T&)> TFilter>
        usize RemoveIf(TFilter&& filter);

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Remove(const T& in);

    //// -------------------------------------------------------------------------------------------
    //// Search
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Contains(const T& in) const noexcept;

    //// -------------------------------------------------------------------------------------------
    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr void Reserve(usize size);

    protected:
        constexpr bool _ValidateIter(TIter it) const noexcept;
        constexpr void _UpdateIterDebugId() noexcept;
        constexpr bool _ValidateIndex(isize index) const noexcept;
        constexpr isize _FetchIndex(TIter pos) const noexcept;
        constexpr usize _CalcCapGrowth(usize required) const noexcept;
        constexpr void _EnsureCapFor(usize count);

        constexpr void _ConstructAt(usize index, auto&&... args);
        constexpr void _DestructAt(usize index);
        constexpr void _DestructRange(usize begin, usize end);
        constexpr void _MoveRangeFront(usize index, usize count);
        constexpr void _MoveRangeBack(usize index, usize count);
        constexpr void _RotateRangeBack(usize index, usize count);

        template <typename TRange>
        static constexpr bool _CanGetRangeSize() noexcept;

        template <typename TRange>
        static constexpr usize _GetRangeSize(const TRange& range) noexcept;

    //// -------------------------------------------------------------------------------------------
    //// Fields
    //// -------------------------------------------------------------------------------------------

    protected:
        T* _arr;
        usize _count;
        usize _capacity;
        usize _iterValidDebugId;
        TMemAlloc _memAllocator;

    private:
        static constexpr ObjHelper<T> _objHelper = ObjHelper<T>();
    };
}