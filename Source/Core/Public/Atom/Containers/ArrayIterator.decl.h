#pragma once
#include "Atom/Containers/InitializerList.h"
#include "Atom/Containers/Iterator.h"

namespace Atom
{
    template <typename T>
    class ArrayIterator
    {
    public:
        /// DefaultConstructor.
        constexpr ArrayIterator() noexcept;

        /// 
        constexpr ArrayIterator(T* begin, usize length) noexcept;

        /// 
        constexpr ArrayIterator(T* begin, T* end) noexcept;

    public:
        /// 
        constexpr T& Get() noexcept;

        /// 
        constexpr bool Next() noexcept;

        /// 
        constexpr bool HasNext() const noexcept;

        /// 
        constexpr bool Prev() noexcept;

        /// 
        constexpr bool HasPrev() const noexcept;

        /// 
        constexpr bool Next(usize steps) noexcept;

        /// 
        constexpr bool Prev(usize steps) noexcept;

        /// 
        constexpr usize NextRange() const noexcept;

        /// 
        constexpr usize PrevRange() const noexcept;

        /// 
        constexpr usize Range() const noexcept;

    public:
        /// 
        constexpr auto begin() noexcept;

        /// 
        constexpr auto end() noexcept;

    protected:
        T* _it;
        T* _begin;
        T* _end;
    };
}