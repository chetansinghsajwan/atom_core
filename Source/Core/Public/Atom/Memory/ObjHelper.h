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

        constexpr void ConstructAt(T* obj, auto&&... args) const
        {
            new (obj) T(FORWARD(args)...);
        }

        constexpr void MoveRange(T* src, usize size, T* dest) const
        {
        }

        constexpr void MoveFwd(T* src, usize count, usize steps) const
        {
            if constexpr (_IsTriviallyMoveConstructible)
            {
                MemHelper().CopySafe(src, count * sizeof(T), src + steps);
                return;
            }

            for (usize i = count; i > 0; i--)
            {
                new (src + i - 1 + count) T(MOVE(src[i - 1]));
            }
        }

        constexpr void DestructAt(T* obj) const
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            obj->T::~T();
        }

        constexpr void DestructRange(T* arr, usize count) const
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            for (usize i = 0; i < count; i++)
            {
                (arr + i)->T::~T();
            }
        }

        constexpr void RotateAntiClock(const T* src, usize mid, usize end) const
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