#pragma once
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

#include <algorithm>

namespace Atom
{
    class RangeHelper
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RRange<TRange>
        constexpr auto CanGetCount() const -> bool
        {
            return RFwdRange<TRange>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RFwdRange<TRange>
        constexpr auto GetCount(const TRange& range) const -> usize
        {
            return _GetCount(range.iter(), range.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange, class T>
            requires RMutRange<TRange> and RAssignable<typename TRange::TElem, T>
        constexpr auto Fill(TRange&& range, T&& val) const -> void
        {
            _Fill(range.iter(), range.iterEnd(), fwd(val));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
#pragma optimize("", off)

        template <class TRange, typename T>
            requires RMutRange<TRange> and RAssignable<typename TRange::TElem, T>
        constexpr auto FillExplicit(TRange&& range, T&& val) const -> void
        {
            _Fill(range.iter(), range.iterEnd(), fwd(val));
        }

#pragma optimize("", on)

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
            requires RRange<TRange1> and RMutRange<TRange2>
                     and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr auto FwdCopyTo(const TRange1& range1, TRange2&& range2) const -> void
        {
            _FwdCopy(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
            requires RRange<TRange1> and RMutRange<TRange2>
                     and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr auto BwdCopyTo(const TRange1& range1, TRange2&& range2) const -> void
        {
            _BwdCopy(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
            requires RMutRange<TRange1> and RMutRange<TRange2>
                     and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr auto FwdMoveTo(TRange1&& range1, TRange2&& range2) const -> void
        {
            _FwdMove(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange1, class TRange2>
            requires RMutRange<TRange1> and RMutRange<TRange2>
                     and RAssignable<typename TRange2::TElem, typename TRange1::TElem>
        constexpr auto BwdMoveTo(TRange1&& range1, TRange2&& range2) const -> void
        {
            _BwdMove(range1.iter(), range1.iterEnd(), range2.iter(), range2.iterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RMutFwdRange<TRange> and RMoveAssignable<typename TRange::TElem>
        constexpr auto ShiftFwd(TRange&& range, usize steps) const -> void
        {
            _FwdShift(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RMutFwdRange<TRange> and RMoveAssignable<typename TRange::TElem>
        constexpr auto ShiftBwd(TRange&& range, usize steps) const -> void
        {
            _BwdShift(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RMutFwdRange<TRange> and RSwappable<typename TRange::TElem>
        constexpr auto ShiftBy(TRange&& range, isize steps) const -> void
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
        template <class TRange>
            requires RMutRange<TRange> and RSwappable<typename TRange::TElem>
        constexpr auto RotateFwd(TRange&& range, usize steps) const -> void
        {
            _FwdRotate(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RMutRange<TRange> and RSwappable<typename TRange::TElem>
        constexpr auto RotateBwd(TRange&& range, usize steps) const -> void
        {
            _BwdRotate(range.iter(), range.iterEnd(), steps);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <class TRange>
            requires RMutRange<TRange> and RSwappable<typename TRange::TElem>
        constexpr auto RotateBy(TRange&& range, isize steps) const -> void
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
        template <class TRange>
            requires RMutRange<TRange> and RDestructible<typename TRange::TElem>
        constexpr auto Destruct(TRange&& range) const -> void
        {
            _Destruct(range.iter(), range.iterEnd());
        }

    private:
        template <class TIter, class TIterEnd>
        constexpr auto _GetCount(TIter iter, TIterEnd iterEnd) const -> usize
        {
            if constexpr (RJumpIterPair<TIter, TIterEnd>)
            {
                return -iter.compare(iterEnd);
            }

            usize count = 0;
            for (; not iter.equals(iterEnd); iter.next())
                count++;

            return count;
        }

        template <class TIter, class TIterEnd, typename T>
        constexpr auto _Fill(TIter iter, TIterEnd iterEnd, T&& val) const -> void
        {
            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::fill(stdIter, stdIterEnd, fwd(val));
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr auto _FwdCopy(TIter1 iter1, TIterEnd1 iterEnd1, TIter2 iter2,
            TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::copy(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr auto _BwdCopy(TIter1 iter1, TIterEnd1 iterEnd1, TIter2 iter2,
            TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::copy_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr auto _FwdMove(TIter1 iter1, TIterEnd1 iterEnd1, TIter2 iter2,
            TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::move(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        constexpr auto _BwdMove(
            TIter1 iter1, TIterEnd1 iterEnd1, TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        {
            StdIterWrapForAtomIter stdIter1{ iter1 };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
            StdIterWrapForAtomIter stdIter2{ iter2 };

            std::move_backward(stdIter1, stdIterEnd1, stdIter2);
        }

        template <class TIter, class TIterEnd>
        constexpr auto _FwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_right(iter.data(), iterEnd.data(), steps);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::shift_right(stdIter, stdIterEnd, steps);
        }

        template <class TIter, class TIterEnd>
        constexpr auto _BwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::shift_left(iter.data(), iterEnd.data(), steps);
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::shift_left(stdIter, stdIterEnd, steps);
        }

        template <class TIter, class TIterEnd>
        constexpr auto _BwdRotate(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(iter.data(), iter.data() + steps, iterEnd.data());
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIter + steps, stdIterEnd);
        }

        template <class TIter, class TIterEnd>
        constexpr auto _FwdRotate(TIter iter, TIterEnd iterEnd, isize steps) const -> void
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::rotate(iter.data(), iterEnd.data() - steps, iterEnd.data());
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::rotate(stdIter, stdIterEnd - steps, stdIterEnd);
        }

        template <class TIter, class TIterEnd>
        constexpr auto _Destruct(TIter iter, TIterEnd iterEnd) const -> void
        {
            if constexpr (RArrIterPair<TIter, TIterEnd>)
            {
                std::destroy(iter.data(), iterEnd.data());
                return;
            }

            StdIterWrapForAtomIter stdIter{ iter };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd };

            std::destroy(stdIter, stdIterEnd);
        }
    };
}
