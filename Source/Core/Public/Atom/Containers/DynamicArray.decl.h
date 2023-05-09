#pragma once
#include <vector>

#include "Atom/Invokable/Invokable.h"
#include "Atom/Containers/Iterable.h"
#include "Atom/Containers/Insertable.h"
#include "Atom/Containers/ArrayIterator.decl.h"

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
        template <typename TInput>
        requires RInputIterator<TInput, T>
        constexpr DynamicArray(TInput in);

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
        template <RInputIterator<T> TInput>
        constexpr TIterator operator += (TInput in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterable<T> TInput>
        constexpr TIterator operator += (TInput in);

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
        template <RInputIterator<T> TInput>
        constexpr TIterator Insert(TIterator pos, TInput in);

        /// Insert element {in} by copy at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(const T& in);

        /// Insert element {in} by move at front.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertFront(T&& in);

        /// Insert element range {in} at front.
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator InsertFront(TInput in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(const T& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator InsertBack(T&& in);

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr TIterator InsertBack(TInput in);

    //// Remove
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        template <RInputIterator<T> TInput>
        constexpr usize Remove(TInput in);

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