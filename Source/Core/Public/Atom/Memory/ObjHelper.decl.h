#pragma once
#include "MemHelper.decl.h"
#include "Atom/Range.h"

namespace Atom
{
    template <typename T>
    struct ObjHelper
    {
        static constexpr bool _IsTriviallyMoveConstructible = 
            std::is_trivially_move_constructible_v<T>;

        static constexpr bool _IsTriviallyDestructible = 
            std::is_trivially_destructible_v<T>;

        static constexpr bool _IsNoexDestructible = 
            std::is_nothrow_destructible_v<T>;

        constexpr void ConstructAt(T* obj, auto&&... args) const
        {
            new (obj) T(FORWARD(args)...);
        }

        template <typename T1Range, typename T2Range>
        requires RMutRangeOf<T1Range, T> && RMutRangeOf<T2Range, T>
        constexpr void FwdMoveRangeTo(T1Range&& range1, T2Range&& range2) const
        {
            return _FwdMove(range1, range2);
        }

        template <typename T1Range, typename T2Range>
        requires RMutRangeOf<T1Range, T> && RMutRangeOf<T2Range, T>
        constexpr void BwdMoveRangeTo(T1Range&& range1, T2Range&& range2) const
        {
            return _BwdMove(range1, range2);
        }

        template <typename T1Range, typename T2Range>
        requires RMutRangeOf<T1Range, T> && RMutRangeOf<T2Range, T>
        constexpr void FwdRotateRange(T1Range&& range1, usize offset) const
        {
        }

        template <typename T1Range, typename T2Range>
        requires RMutRangeOf<T1Range, T> && RMutRangeOf<T2Range, T>
        constexpr void BwdRotateRange(T1Range&& range1, usize offset) const
        {
        }

        template <typename T1Range, typename T2Range>
        requires RMutRangeOf<T1Range, T> && RMutRangeOf<T2Range, T>
        constexpr void RotateRangeBy(T1Range&& range1, usize offset) const
        {
        }

        template <typename TPtr>
        // requires RPtr<TPtr, T>
        constexpr void DestructAt(TPtr&& obj) const noexcept(_IsNoexDestructible)
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            std::destroy_at(obj);
        }

        template <typename TRange>
        requires RMutRangeOf<TRange, T>
        constexpr void DestructRange(TRange&& range) const noexcept(_IsNoexDestructible)
        {
            if constexpr (_IsTriviallyDestructible)
                return;

            // std::destroy(range.Iter(), range.IterEnd());
        }

    private:
        template <typename T1Range>
        constexpr void _FwdMove(T1Range&& range1, auto range2) const
        {
            // std::move(range1.Iter(), range1.IterEnd(), range2);
        }
    };
}