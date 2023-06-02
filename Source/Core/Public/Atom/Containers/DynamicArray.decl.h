#pragma once
#include "Ranges.h"
#include "Range.h"
#include "Output.h"
#include "Insertable.h"
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
        using TIter = ArrayIterator<T>;
        using TIterEnd = TIter;
        using TConstIter = ArrayIterator<const T>;
        using TConstIterEnd = TConstIter;
        // using TRevIter = RevIter<TIter>;
        // using TRevIterEnd = TRevIter;
        // using TConstRevIter = RevIter<TConstIter>;
        // using TConstRevIterEnd = TConstRevIter;
        using TEnd = TIter;

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
        requires RFwdRange<TRange, const T>
        constexpr DynamicArray(const TRange& it);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TIter operator +=(const T& in);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TIter operator +=(T&& in);

        /// ----------------------------------------------------------------------------------------
        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter operator +=(const TRange& it);

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
        constexpr TIter Begin() noexcept
        {
            return TIter{ _arr };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIterEnd} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterEnd End() noexcept
        {
            return TIterEnd{ _arr + _count + 1};
        }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter Begin() const noexcept
        {
            return ConstBegin();
        }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter End() const noexcept
        {
            return ConstEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter ConstBegin() const noexcept
        {
            return TConstIter{ _arr };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter ConstEnd() const noexcept
        {
            return TConstIter{ _arr + _count };
        }

//         /// ----------------------------------------------------------------------------------------
//         /// {TRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TRevIter RevBegin() noexcept
//         {
//             return TRevIter{ _arr + _count };
//         }
// 
//         /// ----------------------------------------------------------------------------------------
//         /// {TRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TRevIter RevEnd() noexcept
//         {
//             return TRevIter{ _arr };
//         }
// 
//         /// ----------------------------------------------------------------------------------------
//         /// {TConstRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter RevBegin() const noexcept
//         {
//             return ConstRevBegin();
//         }
// 
//         /// ----------------------------------------------------------------------------------------
//         /// {TConstRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter RevEnd() const noexcept
//         {
//             return ConstRevEnd();
//         }
// 
//         /// ----------------------------------------------------------------------------------------
//         /// {TConstRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter ConstRevBegin() const noexcept
//         {
//             return TConstRevIter{ _arr + _count };
//         }
// 
//         /// ----------------------------------------------------------------------------------------
//         /// {TConstRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter ConstRevEnd() const noexcept
//         {
//             return TConstRevIter{ _arr };
//         }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter begin() noexcept { return Begin(); }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter end() noexcept { return End(); }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter begin() const noexcept { return Begin(); }

        /// ----------------------------------------------------------------------------------------
        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter end() const noexcept { return End(); }

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
        /// @RETURNS {TIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TIter InsertAt(TIter pos, U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at {pos} pos.
        /// 
        /// @PARAM pos: Position to insert element at.
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertAt(TIter pos, const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at front.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TIter InsertFront(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at front.
        /// 
        /// @PARAM el: Element range to insert.
        /// 
        /// @RETURNS {TIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertFront(const TRange& range);

        /// ----------------------------------------------------------------------------------------
        /// Insert element {el} by forward at back.
        /// 
        /// @PARAM el: Element to insert.
        /// 
        /// @RETURNS {TIter} to the inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename U>
        requires RSameAs<TTI::TRemoveCVRef<U>, T>
        constexpr TIter InsertBack(U&& el);

        /// ----------------------------------------------------------------------------------------
        /// Insert element range {range} at back.
        /// 
        /// @PARAM range: Element range to insert.
        /// 
        /// @RETURNS {TIter} to the first element of inserted range.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertBack(const TRange& range);

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
        /// @RETURNS {TIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter RemoveAt(TConstIter pos);

        /// ----------------------------------------------------------------------------------------
        /// Removes element at range{range}.
        /// 
        /// @PARAM range: Range to remove element at.
        /// 
        /// @RETURNS {TIter} to the next element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter RemoveRange(Range<TConstIter, TConstIterEnd> range);

    private:
        constexpr usize _RemoveAt(usize index);

        constexpr usize _RemoveRange(usize begin, usize count);

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
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
        constexpr bool _ValidateIter(TConstIter it) const noexcept;
        constexpr void _UpdateIterDebugId() noexcept;
        constexpr bool _ValidateIndex(isize index) const noexcept;
        constexpr isize _FetchIndex(TConstIter pos) const noexcept;
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