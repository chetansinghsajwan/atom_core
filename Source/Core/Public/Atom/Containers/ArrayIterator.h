#pragma once
#include "ArrayIterator.decl.h"
#include "Atom/Exceptions.h"

namespace Atom
{
    template <typename T>
    constexpr ArrayIterator<T>::ArrayIterator() noexcept { }

    template <typename T>
    constexpr ArrayIterator<T>::ArrayIterator(T* begin, usize length) noexcept:
        _begin{ begin }, _it{ begin }, _end{ begin + length }
    {
        ATOM_DEBUG_ASSERT(begin != nullptr);
    }

    template <typename T>
    constexpr ArrayIterator<T>::ArrayIterator(T* begin, T* end) noexcept:
        _begin{ begin }, _it{ begin }, _end{ end }
    {
        ATOM_DEBUG_ASSERT(begin != nullptr);
        ATOM_DEBUG_ASSERT(end != nullptr);
        ATOM_DEBUG_ASSERT(begin <= end);
    }

    template <typename T>
    constexpr T& ArrayIterator<T>::Get() noexcept
    {
        return *_it;
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::Next() noexcept
    {
        _it++;
        return HasNext();
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::HasNext() const noexcept
    {
        return _it <= _end;
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::Prev() noexcept
    {
        _it--;
        return HasPrev();
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::HasPrev() const noexcept
    {
        return _it >= _begin;
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::Next(usize steps) noexcept
    {
        _it += steps;
        return HasNext();
    }

    template <typename T>
    constexpr bool ArrayIterator<T>::Prev(usize steps) noexcept
    {
        _it -= steps;
        return HasPrev();
    }

    template <typename T>
    constexpr usize ArrayIterator<T>::NextRange() const noexcept
    {
        return _end - _it;
    }

    template <typename T>
    constexpr usize ArrayIterator<T>::PrevRange() const noexcept
    {
        return _it - _begin;
    }

    template <typename T>
    constexpr usize ArrayIterator<T>::Range() const noexcept
    {
        return _end - _begin;
    }

    template <typename T>
    constexpr auto ArrayIterator<T>::begin() noexcept
    {
        return _it;
    }

    template <typename T>
    constexpr auto ArrayIterator<T>::end() noexcept
    {
        return _end;
    }
}