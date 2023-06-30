#pragma once
#include "_StdIter.h"
#include "RangeReq.h"

#include <algorithm>

namespace Atom
{
    struct RangeHelper
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RRange<TRange>
        constexpr bool CanGetCount() const noexcept
        {
            return RFwdRange<TRange>;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RFwdRange<TRange>
        constexpr usize GetCount(const TRange& range) const noexcept
        {
            return _GetCount(range.Iter(), range.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange, class T>
        requires RMutRange<TRange> && RAssignable<typename TRange::TElem, T>
        constexpr void Fill(TRange&& range, T&& val) const
        {
            _Fill(range.Iter(), range.IterEnd(), FORWARD(val));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
    #pragma optimize("", off)
        template <class TRange, typename T>
        requires RMutRange<TRange> && RAssignable<typename TRange::TElem, T>
        constexpr void FillExplicit(TRange&& range, T&& val) const
        {
            _Fill(range.Iter(), range.IterEnd(), FORWARD(val));
        }
    #pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
        requires RRange<TRange1> && RMutRange<TRange2> &&
            RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr void FwdCopyTo(const TRange1& range1, TRange2&& range2) const
        {
            _FwdCopy(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
        requires RRange<TRange1> && RMutRange<TRange2> &&
            RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr void BwdCopyTo(const TRange1& range1, TRange2&& range2) const
        {
            _BwdCopy(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
        requires RMutRange<TRange1> && RMutRange<TRange2> &&
            RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr void FwdMoveTo(TRange1&& range1, TRange2&& range2) const
        {
            _FwdMove(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
        requires RMutRange<TRange1> && RMutRange<TRange2> &&
            RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr void BwdMoveTo(TRange1&& range1, TRange2&& range2) const
        {
            _BwdMove(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutFwdRange<TRange> && RMoveAssignable<typename TRange::TElem>
        constexpr void ShiftFwd(TRange&& range, usize steps) const
        {
            _FwdShift(range.Iter(), range.IterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutFwdRange<TRange> && RMoveAssignable<typename TRange::TElem>
        constexpr void ShiftBwd(TRange&& range, usize steps) const
        {
            _BwdShift(range.Iter(), range.IterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutFwdRange<TRange> && RSwappable<typename TRange::TElem>
        constexpr void ShiftBy(TRange&& range, isize steps) const
        {
            if (steps > 0)
            {
                _BwdShift(range.Iter(), range.IterEnd(), steps);
            }
            else
            {
                _FwdShift(range.Iter(), range.IterEnd(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutRange<TRange> && RSwappable<typename TRange::TElem>
        constexpr void RotateFwd(TRange&& range, usize steps) const
        {
            _FwdRotate(range.Iter(), range.IterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutRange<TRange> && RSwappable<typename TRange::TElem>
        constexpr void RotateBwd(TRange&& range, usize steps) const
        {
            _BwdRotate(range.Iter(), range.IterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutRange<TRange> && RSwappable<typename TRange::TElem>
        constexpr void RotateBy(TRange&& range, isize steps) const
        {
            if (steps > 0)
            {
                _BwdRotate(range.Iter(), range.IterEnd(), steps);
            }
            else
            {
                _FwdRotate(range.Iter(), range.IterEnd(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
        requires RMutRange<TRange> && RDestructible<typename TRange::TElem>
        constexpr void Destruct(TRange&& range) const
        {
            _Destruct(range.Iter(), range.IterEnd());
        }

    private:
        template <class TIter, class TIterEnd>
        constexpr usize _GetCount(TIter iter, TIterEnd iterEnd) const
        {
            if constexpr (RJumpIterPair<TIter, TIterEnd>)
            {
                return iterEnd - iter;
            }

            usize count = 0;
            for (; iter != iterEnd; iter++) count++;

            return count;
        }

        template <class TIter, class TIterEnd>
        constexpr void _Fill(TIter iter, TIterEnd iterEnd, const class TIter::TElem& val) const
        {
            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::fill(stdIter, stdIterEnd, val);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr void _FwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const
        {
            _StdIterWrapForAtom stdIter1{ iter1 };
            _StdIterWrapForAtom stdIterEnd1{ iterEnd1 };
            _StdIterWrapForAtom stdIter2{ iter2 };
            _StdIterWrapForAtom stdIterEnd2{ iterEnd2 };

            std::copy(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr void _BwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const
        {
            _StdIterWrapForAtom stdIter1{ iter1 };
            _StdIterWrapForAtom stdIterEnd1{ iterEnd1 };
            _StdIterWrapForAtom stdIter2{ iter2 };
            _StdIterWrapForAtom stdIterEnd2{ iterEnd2 };

            std::copy_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr void _FwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const
        {
            _StdIterWrapForAtom stdIter1{ iter1 };
            _StdIterWrapForAtom stdIterEnd1{ iterEnd1 };
            _StdIterWrapForAtom stdIter2{ iter2 };
            _StdIterWrapForAtom stdIterEnd2{ iterEnd2 };

            std::move(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr void _BwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const
        {
            _StdIterWrapForAtom stdIter1{ iter1 };
            _StdIterWrapForAtom stdIterEnd1{ iterEnd1 };
            _StdIterWrapForAtom stdIter2{ iter2 };
            _StdIterWrapForAtom stdIterEnd2{ iterEnd2 };

            std::move_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter, class TIterEnd>
        constexpr void _FwdShift(TIter iter, TIterEnd iterEnd, usize steps) const
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_right(&*iter, &*iterEnd, steps);
                return;
            }

            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::shift_right(stdIter, stdIterEnd, steps);
        }

        template <class TIter, class TIterEnd>
        constexpr void _BwdShift(TIter iter, TIterEnd iterEnd, usize steps) const
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_left(&*iter, &*iterEnd, steps);
                return;
            }

            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::shift_left(stdIter, stdIterEnd, steps);
        }

        template <class TIter, class TIterEnd>
        constexpr void _BwdRotate(TIter iter, TIterEnd iterEnd, usize steps) const
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(&*iter, &*iter + steps, &*iterEnd);
                return;
            }

            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIter + steps, stdIterEnd);
        }

        template <class TIter, class TIterEnd>
        constexpr void _FwdRotate(TIter iter, TIterEnd iterEnd, isize steps) const
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(&*iter, &*iterEnd - steps, &*iterEnd);
                return;
            }

            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIterEnd - steps, stdIterEnd);
        }

        template <class TIter, class TIterEnd>
        constexpr void _Destruct(TIter iter, TIterEnd iterEnd) const
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::destroy(&*iter, &*iterEnd);
                return;
            }

            _StdIterWrapForAtom stdIter{ iter };
            _StdIterWrapForAtom stdIterEnd{ iterEnd };

            std::destroy(stdIter, stdIterEnd);
        }
    };
}