#pragma once
#include "Atom/Exceptions.h"

#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename T>
    class ArrayView
    {
    public:
        using TIterator = ArrayIterator<const T>;

    //// -------------------------------------------------------------------------------------------
    //// Constructors, Operators and Destructor.
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr ArrayView() noexcept:
            m_arr{ nullptr }, m_count{ 0 } { }

        constexpr ArrayView(const T* in_begin, const T* in_end) noexcept:
            m_arr{ in_begin }, m_count{ usize(in_end - in_begin) }
        {
            ATOM_DEBUG_ASSERT(in_begin != nullptr);
            ATOM_DEBUG_ASSERT(in_end != nullptr);
            ATOM_DEBUG_ASSERT(in_begin <= in_end);
        }

        constexpr ArrayView(const T* in_arr, usize in_count) noexcept:
            m_arr{ in_arr }, m_count{ in_count }
        {
            ATOM_DEBUG_ASSERT(in_arr != nullptr);
        }

        template <RInputIterator<T> TInput>
        constexpr bool operator == (TInput in) const noexcept
        {
            usize i = 0;
            for (const T& element : in)
            {
                if (i == Count())
                {
                    return false;
                    break;
                }

                if (element != m_arr[i])
                {
                    return false;
                }

                i++;
            }

            return true;
        }

        constexpr bool operator == (const ArrayView& in) const noexcept
        {
            if (Count() != in.Count())
                return false;

            for (usize i = 0; i < Count(); i++)
            {
                if (m_arr[i] != in.m_arr[i])
                    return false;
            }

            return true;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get Iterator.
        /// ----------------------------------------------------------------------------------------
        constexpr TIterator Iterator() const noexcept
        {
            return TIterator{ m_arr, m_count };
        }

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get pointer to underlying array.
        /// ----------------------------------------------------------------------------------------
        constexpr const T* Data() const noexcept
        {
            return m_arr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr usize Count() const noexcept
        {
            return m_count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr bool IsEmpty() const noexcept
        {
            return Count() == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Fields.
    //// -------------------------------------------------------------------------------------------

    protected:
        const T* m_arr;
        usize m_count;
    };
}