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
        using STD_TVector = ::std::vector<T>;

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
        template <typename TIt>
        requires RFwdIt<TIt, T>
        constexpr DynamicArray(TIt it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(const STD_TVector& vec) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr DynamicArray(STD_TVector&& vec) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator operator += (T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<T> TIt>
        constexpr TIterator operator += (TIt it);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdRange<T> TIt>
        constexpr TIterator operator += (TIt it);

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
        template <RFwdIt<T> TIt>
        constexpr TIterator Insert(TIterator pos, TIt it);

        /// Insert element {in} by copy at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(const T& in);

        /// Insert element {in} by move at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(T&& in);

        /// Insert element range {in} at front.
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<T> TIt>
        constexpr TIterator InsertFront(TIt it);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<T> TIt>
        constexpr TIterator InsertBack(TIt it);

    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<T> TIt>
        constexpr usize Remove(TIt it);

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
        STD_TVector _vector;
    };
}