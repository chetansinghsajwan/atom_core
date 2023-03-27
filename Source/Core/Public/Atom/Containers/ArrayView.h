#pragma once
#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename TElement>
    class ArrayView
    {
    public:
        constexpr ArrayView(TElement* in_begin, TElement* in_end) noexcept :
            m_arr{ in_begin }, m_count{ in_end - in_begin } { }

        constexpr ArrayView(TElement* in_arr, usize in_count) noexcept :
            m_arr{ in_arr }, m_count{ in_count } { }

        //// -------------------------------------------------------------------------------------------
        //// Iteration.
        //// -------------------------------------------------------------------------------------------

    public:
        /// --------------------------------------------------------------------------------------------
        /// Get {ConstIterator} to the beginning of the array.
        /// 
        /// @{

        constexpr ConstArrayIterator<TElement> ConstBegin() const noexcept
        {
            return m_arr;
        }

        constexpr ConstArrayIterator<TElement> Begin() const noexcept
        {
            return ConstBegin();
        }

        constexpr ConstArrayIterator<TElement> begin() const noexcept
        {
            return ConstBegin();
        }

        /// @}
        /// --------------------------------------------------------------------------------------------

        /// --------------------------------------------------------------------------------------------
        /// Get {ConstIterator} to the end of the array.
        /// 
        /// @{

        constexpr ConstArrayIterator<TElement> ConstEnd() const noexcept
        {
            return m_arr + m_count;
        }

        constexpr ConstArrayIterator<TElement> End() const noexcept
        {
            return ConstEnd();
        }

        constexpr ConstArrayIterator<TElement> end() const noexcept
        {
            return ConstEnd();
        }

        /// @}
        /// --------------------------------------------------------------------------------------------

        //// -------------------------------------------------------------------------------------------
        //// Fields.
        //// -------------------------------------------------------------------------------------------

    protected:
        TElement* m_arr;
        usize m_count;
    };
}