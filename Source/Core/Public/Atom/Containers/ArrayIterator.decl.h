#pragma once
#include "Atom/Containers/InitializerList.h"
#include "Atom/Containers/Iterators.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// ArrayIterator iterates over raw array.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterator
    {
    public:
        /// DefaultConstructor.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator() noexcept;

        /// Constructor. Initializes with range {[begin, begin + length]}.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(T* begin, usize length) noexcept;

        /// Constructor. Initializes with range {[begin, end]}.
        /// 
        /// @THROWS InvalidArgumentException In Debug If {begin > end}.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(T* begin, T* end) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <usize count>
        constexpr ArrayIterator(T(&arr)[count]) noexcept:
            ArrayIterator(arr, count) { }

        /// Constructor. Initializes with range {[list.begin(), list.size()]}.
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayIterator(InitializerList<TTI::TRemoveConst<T>> list) noexcept:
            ArrayIterator(list.begin(), list.size()) { }

    public:
        /// Reference to current element.
        /// ----------------------------------------------------------------------------------------
        constexpr T& Get() noexcept;

        /// Move to next element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool Next() noexcept;

        /// Checks if there is next element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool HasNext() const noexcept;

        /// Move to previous element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool Prev() noexcept;

        /// Check if there is previous element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool HasPrev() const noexcept;

        /// Move to next element which is {steps} steps away from current element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool Next(usize steps) noexcept;

        /// Move to previous element which is {steps} steps behind from current element.
        /// ----------------------------------------------------------------------------------------
        constexpr bool Prev(usize steps) noexcept;

        /// Get count of elements remaining forward.
        /// ----------------------------------------------------------------------------------------
        constexpr usize NextRange() const noexcept;

        /// Get count of elements remaining backward.
        /// ----------------------------------------------------------------------------------------
        constexpr usize PrevRange() const noexcept;

        /// Get total count of elements this iterator iterates on.
        /// ----------------------------------------------------------------------------------------
        constexpr usize Range() const noexcept;

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() noexcept;

    protected:
        T* _it;
        T* _begin;
        T* _end;
    };
}