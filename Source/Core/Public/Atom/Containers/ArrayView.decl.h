#pragma once
#include "Atom/Containers/ArrayIterator.decl.h"

namespace Atom
{
    template <typename T>
    class ArrayView
    {
    public:
        using TIterator = ArrayIterator<const T>;

    //// Constructors, Operators and Destructor.
    //// -------------------------------------------------------------------------------------------

    public:
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView() noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView(const T* in_begin, const T* in_end) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr ArrayView(const T* in_arr, usize in_count) noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        template <RFwdIt<T> TInput>
        constexpr bool operator == (TInput in) const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr bool operator == (const ArrayView& in) const noexcept;

        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr const T& operator [] (usize index) const noexcept;

    //// Iteration.
    //// -------------------------------------------------------------------------------------------

    public:
        /// Get Iterator.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Iterator() const noexcept;

    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// Get pointer to underlying array.
        /// ----------------------------------------------------------------------------------------
        constexpr const T* Data() const noexcept;

        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr usize Count() const noexcept;

        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr bool IsEmpty() const noexcept;

    //// Fields.
    //// -------------------------------------------------------------------------------------------

    protected:
        const T* m_arr;
        usize m_count;
    };
}