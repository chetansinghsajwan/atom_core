#pragma once
#include "Atom/Containers/ArrayIterator.h"

namespace Atom
{
    template <typename T, usize Size>
    class StaticArray
    {
    public:
        using TIterator = ArrayIterator<T>;
        using TConstIterator = ArrayIterator<const T>;

    public:
        constexpr bool operator == (const StaticArray& other) const noexcept
        {
            for (usize i = 0; i < Size; i++)
            {
                if (arr[i] != other.arr[i])
                    return false;
            }

            return true;
        }

        constexpr bool operator != (const StaticArray& other) const noexcept
        {
            return !(*this == other);
        }

    public:
        constexpr T& operator [] (usize index) noexcept
        {
            return arr[index];
        }

        constexpr const T& operator [] (usize index) const noexcept
        {
            return arr[index];
        }

    public:
        constexpr T* Data() noexcept
        {
            return arr;
        }

        constexpr const T* Data() const noexcept
        {
            return arr;
        }

        constexpr usize Count() const noexcept
        {
            return Size;
        }

    public:
        constexpr TIterator Iterator() noexcept
        {
            return TIterator{ arr, Size };
        }

        constexpr TConstIterator Iterator() const noexcept
        {
            return TConstIterator{ arr, Size };
        }

        constexpr TConstIterator ConstIterator() const noexcept
        {
            return TConstIterator{ arr, Size };
        }

    public:
        T arr[Size];
    };
}