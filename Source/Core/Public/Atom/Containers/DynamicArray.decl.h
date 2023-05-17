#pragma once
#include <vector>

#include "Ranges.h"
#include "Output.h"
#include "Insertable.h"
#include "ArrayIterator.decl.h"
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
        using TIterator = ArrayIterator<T>;
        using TConstIterator = ArrayIterator<const T>;

    //// Constructors, Operators and Destructor
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const DynamicArray& in) = default;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(DynamicArray&& in) = default;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TIter>
        requires RFwdIter<TIter, const T>
        constexpr DynamicArray(TIter it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const _TVector& vec) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(_TVector&& vec) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TIter>
        requires RFwdIter<TIter, const T>
        constexpr TIterator operator += (TIter it);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange, const T>
        constexpr TIterator operator += (const TRange& it);

        /// 
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

    //// Iterator
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Iterator() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TConstIterator Iterator() const noexcept;

    //// Insert
    //// -------------------------------------------------------------------------------------------

    public:
        /// Insert element {in} by copy at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element to insert.
        /// 
        /// @RETURNS {TIterator} to inserted element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Insert(TIterator pos, const T& in);

        /// Insert element {in} by move at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element to insert.
        /// 
        /// @RETURNS {TIterator} to inserted element.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Insert(TIterator pos, T&& in);

        /// Insert element range {in} at {pos} pos.
        /// 
        /// @PARAM[IN] pos: Position to insert element at.
        /// @PARAM[IN] in: Element range to insert.
        /// 
        /// @RETURNS {TIterator} to inserted element.
        /// ----------------------------------------------------------------------------------------
        template <RFwdIter<T> TIter>
        constexpr TIterator Insert(TIterator pos, TIter it);

        /// Insert element {in} by copy at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(const T& in);

        /// Insert element {in} by move at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(T&& in);

        /// Insert element range {in} at front.
        /// ----------------------------------------------------------------------------------------
        template <RFwdIter<T> TIter>
        constexpr TIterator InsertFront(TIter it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TIter>
        requires RFwdIter<TIter, const T>
        constexpr TIterator InsertBack(TIter it);

    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdIter<T> TIter>
        constexpr usize Remove(TIter it);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInvokable<bool(const T&)> TFilter>
        usize RemoveIf(TFilter&& filter);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool Remove(TIterator it);

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