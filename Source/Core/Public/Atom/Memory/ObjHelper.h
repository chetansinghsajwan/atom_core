#pragma once
#include "MemHelper.h"

namespace Atom
{
    template <typename T>
    struct ObjHelper
    {
        static constexpr bool _IsTriviallyMoveConstructible = 
            std::is_trivially_move_constructible_v<T>;

        static constexpr bool _IsTriviallyDestructible = 
            std::is_trivially_destructible_v<T>;

        constexpr void ConstructAt(T* obj, auto&&... args) const noexcept
        {
            new (obj) T(FORWARD(args)...);
        }

        constexpr void MoveRange(const T* src, usize size, T* dest) const noexcept
        {
        }

        constexpr void MoveFwd(const T* src, usize count, usize steps) const noexcept
        {
            if constexpr (_IsTriviallyMoveConstructible)
            {
                MemHelper().CopyFwd(src, count * sizeof(T), steps);
                return;
            }

            for (usize i = count; i > 0; i--)
            {
                src[i - 1 + count] = MOVE(src[i - 1]);
            }
        }

        constexpr void DestructAt(T* obj)
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            obj->T::~T();
        }

        constexpr void DestructRange(T* arr, usize count)
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            for (usize i = 0; i < count; i++)
            {
                (arr + i)->T::~T();
            }
        }

        constexpr void RotateAntiClock(const T* src, usize mid, usize end) const noexcept
        {
            if constexpr (_IsTriviallyMoveConstructible)
            {
                // TODO: Refactor this.
                // NOTE: Keep std implementation, MSVC uses vectorization for this.
                std::rotate(src, src + mid, src + end);
            }
        }
    };
}