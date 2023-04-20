#pragma once
#include "Atom/Exceptions.h"

#include "Atom/Containers/InitializerList.h"

namespace Atom
{
    template <typename T>
    class ArrayIterator
    {
    public:
        constexpr ArrayIterator() noexcept { }

        constexpr ArrayIterator(T* begin, usize length) noexcept:
            _begin{ begin }, _it{ begin }, _end{ begin + length }
        {
            ATOM_DEBUG_ASSERT(begin != nullptr);
        }

        constexpr ArrayIterator(T* begin, T* end) noexcept:
            _begin{ begin }, _it{ begin }, _end{ end }
        {
            ATOM_DEBUG_ASSERT(begin != nullptr);
            ATOM_DEBUG_ASSERT(end != nullptr);
            ATOM_DEBUG_ASSERT(begin <= end);
        }

        // constexpr ArrayIterator(InitializerList<T> list) noexcept:
        //     ArrayIterator(list.begin(), list.end()) { }

    public:
        constexpr T& Get() noexcept
        {
            return *_it;
        }

        constexpr bool Next() noexcept
        {
            _it++;
            return HasNext();
        }

        constexpr bool HasNext() const noexcept
        {
            return _it <= _end;
        }

        constexpr bool Prev() noexcept
        {
            _it--;
            return HasPrev();
        }

        constexpr bool HasPrev() const noexcept
        {
            return _it >= _begin;
        }

        constexpr bool Next(usize steps) noexcept
        {
            _it += steps;
            return HasNext();
        }

        constexpr bool Prev(usize steps) noexcept
        {
            _it -= steps;
            return HasPrev();
        }

        constexpr usize NextRange() const noexcept
        {
            return _end - _it;
        }

        constexpr usize PrevRange() const noexcept
        {
            return _it - _begin;
        }

        constexpr usize Range() const noexcept
        {
            return _end - _begin;
        }

    public:
        constexpr auto begin() noexcept
        {
            return _it;
        }

        constexpr auto end() noexcept
        {
            return _end;
        }

    protected:
        T* _it;
        T* _begin;
        T* _end;
    };
}