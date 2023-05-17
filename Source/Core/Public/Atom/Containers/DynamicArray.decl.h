#pragma once
#include <vector>

#include "Ranges.h"
#include "Output.h"
#include "Insertable.h"
#include "ArrayIterator.h"
#include "Atom/Invokable/Invokable.h"

namespace Atom
{
    template <typename T>
    using DefaultAllocator = ::std::allocator<T>;

    template <typename T, typename TAllocator = DefaultAllocator<T>>
    class DynamicArray
    {
        using _TVector = ::std::vector<T>;

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

    //// Constructors, Operators and Destructor
    //// -------------------------------------------------------------------------------------------

    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray() noexcept;

        /// CopyConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const DynamicArray& in);

        /// MoveConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(DynamicArray&& in) noexcept;

        /// ParameterizedConstructor.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr DynamicArray(const TRange& it);

        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TIter operator += (const T& in);

        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        constexpr TIter operator += (T&& in);

        /// Calls InsertBack().
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter operator += (const TRange& it);

        /// Destructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ~DynamicArray();

    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr T* Data() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T* Data() const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr usize Count() const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool IsEmpty() const noexcept;

    //// Iter
    //// -------------------------------------------------------------------------------------------

    public:
        /// {TIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter Begin() noexcept
        {
            return TIter{ _vector.data() };
        }

        /// {TIterEnd} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterEnd End() noexcept
        {
            return TIterEnd{ _vector.data() + _vector.size() + 1};
        }

        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter Begin() const noexcept
        {
            return ConstBegin();
        }

        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter End() const noexcept
        {
            return ConstEnd();
        }

        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter ConstBegin() const noexcept
        {
            return TConstIter{ _vector.data() };
        }

        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter ConstEnd() const noexcept
        {
            return TConstIter{ _vector.data() + _vector.size() + 1};
        }

//         /// {TRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TRevIter RevBegin() noexcept
//         {
//             return TRevIter{ _vector.data() + _vector.size() };
//         }
// 
//         /// {TRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TRevIter RevEnd() noexcept
//         {
//             return TRevIter{ _vector.data() };
//         }
// 
//         /// {TConstRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter RevBegin() const noexcept
//         {
//             return ConstRevBegin();
//         }
// 
//         /// {TConstRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter RevEnd() const noexcept
//         {
//             return ConstRevEnd();
//         }
// 
//         /// {TConstRevIter} to first element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter ConstRevBegin() const noexcept
//         {
//             return TConstRevIter{ _vector.data() + _vector.size() };
//         }
// 
//         /// {TConstRevIter} to last element.
//         /// ----------------------------------------------------------------------------------------
//         constexpr TConstRevIter ConstRevEnd() const noexcept
//         {
//             return TConstRevIter{ _vector.data() };
//         }

        /// {TIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter begin() noexcept { return Begin(); }

        /// {TIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter end() noexcept { return End(); }

        /// {TConstIter} to first element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter begin() const noexcept { return Begin(); }

        /// {TConstIter} to last element.
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIter end() const noexcept { return End(); }

    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// Insert element {in} by copy at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element to insert.
        /// 
        /// @RETURNS {TIter} to inserted element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertAt(TIter pos, const T& in);

        /// Insert element {in} by move at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element to insert.
        /// 
        /// @RETURNS {TIter} to inserted element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertAt(TIter pos, T&& in);

        /// Insert element range {in} at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element range to insert.
        /// 
        /// @RETURNS {TIter} to inserted element.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertAt(TIter pos, const TRange& it);

        /// Insert element {in} by copy at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertFront(const T& in);

        /// Insert element {in} by move at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertFront(T&& in);

        /// Insert element range {in} at front.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertFront(const TRange& it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertBack(const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIter InsertBack(T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIter InsertBack(const TRange& range);

    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr usize Remove(const TRange& it);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<bool(const T&)> TFilter>
        usize RemoveIf(TFilter&& filter);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Remove(TIter it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Remove(const T& in);

    //// Search
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Contains(const T& in) const noexcept;

    //// Memory
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr void Reserve(usize size);

    //// Fields
    //// -------------------------------------------------------------------------------------------

    private:
        _TVector _vector;
    };
}