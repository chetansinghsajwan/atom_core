#pragma once
#include "StdIterWrapForAtomIter.h"
#include "RangeReq.h"

#include <algorithm>

namespace Atom
{
    class RangeHelper
    {
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RRange<TRange>
        cexpr fn CanGetCount() const noex -> bool
        {
            return RFwdRange<TRange>;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RFwdRange<TRange>
        cexpr fn GetCount(const TRange& range) const noex -> usize
        {
            return _GetCount(range.iter(), range.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange, class T>
        requires RMutRange<TRange>
            and RAssignable<typename TRange::TElem, T>
        cexpr fn Fill(TRange&& range, T&& val) const -> void
        {
            _Fill(range.iter(), range.iterEnd(), fwd(val));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
    #pragma optimize("", off)
        pub template <class TRange, typename T>
        requires RMutRange<TRange>
            and RAssignable<typename TRange::TElem, T>
        cexpr fn FillExplicit(TRange&& range, T&& val) const -> void
        {
            _Fill(range.iter(), range.iterEnd(), fwd(val));
        }
    #pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange1, class TRange2>
        requires RRange<TRange1>
            and RMutRange<TRange2>
            and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        cexpr fn FwdCopyTo(const TRange1& range1, TRange2&& range2) const -> void
        {
            _FwdCopy(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange1, class TRange2>
        requires RRange<TRange1>
            and RMutRange<TRange2>
            and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        cexpr fn BwdCopyTo(const TRange1& range1, TRange2&& range2) const -> void
        {
            _BwdCopy(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange1, class TRange2>
        requires RMutRange<TRange1>
            and RMutRange<TRange2>
            and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        cexpr fn FwdMoveTo(TRange1&& range1, TRange2&& range2) const -> void
        {
            _FwdMove(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange1, class TRange2>
        requires RMutRange<TRange1>
            and RMutRange<TRange2>
            and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        cexpr fn BwdMoveTo(TRange1&& range1, TRange2&& range2) const -> void
        {
            _BwdMove(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutFwdRange<TRange>
            and RMoveAssignable<typename TRange::TElem>
        cexpr fn ShiftFwd(TRange&& range, usize steps) const -> void
        {
            _FwdShift(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutFwdRange<TRange>
            and RMoveAssignable<typename TRange::TElem>
        cexpr fn ShiftBwd(TRange&& range, usize steps) const -> void
        {
            _BwdShift(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutFwdRange<TRange>
            and RSwappable<typename TRange::TElem>
        cexpr fn ShiftBy(TRange&& range, isize steps) const -> void
        {
            if (steps > 0)
            {
                _BwdShift(range.iter(), range.iterEnd(), steps);
            }
            else
            {
                _FwdShift(range.iter(), range.iterEnd(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutRange<TRange>
            and RSwappable<typename TRange::TElem>
        cexpr fn RotateFwd(TRange&& range, usize steps) const -> void
        {
            _FwdRotate(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutRange<TRange>
            and RSwappable<typename TRange::TElem>
        cexpr fn RotateBwd(TRange&& range, usize steps) const -> void
        {
            _BwdRotate(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutRange<TRange>
            and RSwappable<typename TRange::TElem>
        cexpr fn RotateBy(TRange&& range, isize steps) const -> void
        {
            if (steps > 0)
            {
                _BwdRotate(range.iter(), range.iterEnd(), steps);
            }
            else
            {
                _FwdRotate(range.iter(), range.iterEnd(), -steps);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub template <class TRange>
        requires RMutRange<TRange>
            and RDestructible<typename TRange::TElem>
        cexpr fn Destruct(TRange&& range) const -> void
        {
            _Destruct(range.iter(), range.iterEnd());
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _GetCount(TIter iter, TIterEnd iterEnd) const -> usize
        {
            if cexpr (RJumpIterPair<TIter, TIterEnd>)
            {
                return iterEnd - iter;
            }

            usize count = 0;
            for (; iter != iterEnd; iter++) count++;

            return count;
        }

        priv template <class TIter, class TIterEnd, typename T>
        cexpr fn _Fill(TIter iter, TIterEnd iterEnd, T&& val) const -> void
        {
            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::fill(stdIter, stdIterEnd, fwd(val));
        }

        priv template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        cexpr fn _FwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::copy(stdIter1, stdIterEnd1, stdIter2);
        }

        priv template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        cexpr fn _BwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::copy_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        priv template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        cexpr fn _FwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::move(stdIter1, stdIterEnd1, stdIter2);
        }

        priv template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        cexpr fn _BwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
            TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::move_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _FwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if cexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_right(&*iter, &*iterEnd, steps);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::shift_right(stdIter, stdIterEnd, steps);
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _BwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if cexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_left(&*iter, &*iterEnd, steps);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::shift_left(stdIter, stdIterEnd, steps);
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _BwdRotate(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if cexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(&*iter, &*iter + steps, &*iterEnd);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIter + steps, stdIterEnd);
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _FwdRotate(TIter iter, TIterEnd iterEnd, isize steps) const -> void
        {
            if cexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(&*iter, &*iterEnd - steps, &*iterEnd);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIterEnd - steps, stdIterEnd);
        }

        priv template <class TIter, class TIterEnd>
        cexpr fn _Destruct(TIter iter, TIterEnd iterEnd) const -> void
        {
            if cexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::destroy(&*iter, &*iterEnd);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::destroy(stdIter, stdIterEnd);
        }
    };
}