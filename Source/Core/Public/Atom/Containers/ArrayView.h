#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Containers/ArrayIterator.h"
#include "ArrayView.decl.h"

namespace Atom
{
    template <typename T>
    constexpr ArrayView<T>::ArrayView() noexcept:
        m_arr{ nullptr }, m_count{ 0 } { }

    template <typename T>
    constexpr ArrayView<T>::ArrayView(const T* in_begin, const T* in_end) noexcept:
        m_arr{ in_begin }, m_count{ usize(in_end - in_begin) }
    {
        ATOM_DEBUG_EXPECTS(in_begin != nullptr);
        ATOM_DEBUG_EXPECTS(in_end != nullptr);
        ATOM_DEBUG_EXPECTS(in_begin <= in_end);
    }

    template <typename T>
    constexpr ArrayView<T>::ArrayView(const T* in_arr, usize in_count) noexcept:
        m_arr{ in_arr }, m_count{ in_count }
    {
        ATOM_DEBUG_EXPECTS(in_arr != nullptr);
    }

    template <typename T>
    template <typename TRange>
    requires RRange<TRange, T>
    constexpr bool ArrayView<T>::operator == (const TRange& range) const noexcept
    {
        usize i = 0;
        for (const T& element : range)
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

    template <typename T>
    constexpr bool ArrayView<T>::operator == (const ArrayView& in) const noexcept
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

    template <typename T>
    constexpr const T& ArrayView<T>::operator [] (usize index) const noexcept
    {
        return m_arr[index];
    }

    template <typename T>
    constexpr const T* ArrayView<T>::Data() const noexcept
    {
        return m_arr;
    }

    template <typename T>
    constexpr usize ArrayView<T>::Count() const noexcept
    {
        return m_count;
    }

    template <typename T>
    constexpr bool ArrayView<T>::IsEmpty() const noexcept
    {
        return Count() == 0;
    }
}