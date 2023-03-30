#pragma once
// #include "Atom/Exceptions.h"

#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename TElement>
    class ArrayView
    {
    public:
        constexpr ArrayView(const TElement* in_begin, const TElement* in_end) noexcept:
            m_arr{ in_begin }, m_count{ usize(in_end - in_begin) }
        {
            // TODO: Resolve circular dependencies.
            // ATOM_DEBUG_ASSERT(in_end > in_begin, "Invalid values.");
        }

        constexpr ArrayView(const TElement* in_arr, usize in_count) noexcept:
            m_arr{ in_arr }, m_count{ in_count } { }

    //// -------------------------------------------------------------------------------------------
    //// Iteration.
    //// -------------------------------------------------------------------------------------------

    public:
    /// --------------------------------------------------------------------------------------------
    /// Get {ConstIterator} to the beginning of the array.
    /// 
    /// @{

        constexpr ArrayIterator<const TElement> ConstBegin() const noexcept
        {
            return m_arr;
        }

        constexpr ArrayIterator<const TElement> Begin() const noexcept
        {
            return ConstBegin();
        }

        constexpr ArrayIterator<const TElement> begin() const noexcept
        {
            return ConstBegin();
        }

    /// @}
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Get {ConstIterator} to the end of the array.
    /// 
    /// @{

        constexpr ArrayIterator<const TElement> ConstEnd() const noexcept
        {
            return m_arr + m_count;
        }

        constexpr ArrayIterator<const TElement> End() const noexcept
        {
            return ConstEnd();
        }

        constexpr ArrayIterator<const TElement> end() const noexcept
        {
            return ConstEnd();
        }

    /// @}
    /// --------------------------------------------------------------------------------------------

    //// -------------------------------------------------------------------------------------------
    //// Fields.
    //// -------------------------------------------------------------------------------------------

    protected:
        const TElement* m_arr;
        usize m_count;
    };
}