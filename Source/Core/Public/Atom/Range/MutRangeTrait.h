#pragma once
#include "RangeTrait.h"

namespace Atom
{
    template <typename T>
    class MutRangeTraitImpl
    {};

    template <typename T>
    class _MutRangeTraitImpl
    {
    private:
        // template <class TIter, class TIterEnd, typename T>
        // constexpr auto _Fill(TIter iter, TIterEnd iterEnd, T&& val) const -> void
        // {
        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::fill(stdIter, stdIterEnd, fwd(val));
        // }

        // template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        // constexpr auto _FwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
        //     TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        // {
        //     StdIterWrapForAtomIter stdIter1{ iter1 };
        //     StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
        //     StdIterWrapForAtomIter stdIter2{ iter2 };

        //     std::copy(stdIter1, stdIterEnd1, stdIter2);
        // }

        // template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        // constexpr auto _BwdCopy(const TIter1 iter1, const TIterEnd1 iterEnd1,
        //     TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        // {
        //     StdIterWrapForAtomIter stdIter1{ iter1 };
        //     StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
        //     StdIterWrapForAtomIter stdIter2{ iter2 };

        //     std::copy_backward(stdIter1, stdIterEnd1, stdIter2);
        // }

        // template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        // constexpr auto _FwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
        //     TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        // {
        //     StdIterWrapForAtomIter stdIter1{ iter1 };
        //     StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
        //     StdIterWrapForAtomIter stdIter2{ iter2 };

        //     std::move(stdIter1, stdIterEnd1, stdIter2);
        // }

        // template <class TIter1, class TIterEnd1, class TIter2, class TIterEnd2>
        // constexpr auto _BwdMove(TIter1 iter1, const TIterEnd1 iterEnd1,
        //     TIter2 iter2, TIterEnd2 iterEnd2) const -> void
        // {
        //     StdIterWrapForAtomIter stdIter1{ iter1 };
        //     StdIterWrapForAtomIter stdIterEnd1{ iterEnd1 };
        //     StdIterWrapForAtomIter stdIter2{ iter2 };

        //     std::move_backward(stdIter1, stdIterEnd1, stdIter2);
        // }

        // template <class TIter, class TIterEnd>
        // constexpr auto _FwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        // {
        //     if constexpr (RArrIterPair<TIter, TIterEnd>)
        //     {
        //         std::shift_right(&*iter, &*iterEnd, steps);
        //         return;
        //     }

        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::shift_right(stdIter, stdIterEnd, steps);
        // }

        // template <class TIter, class TIterEnd>
        // constexpr auto _BwdShift(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        // {
        //     if constexpr (RArrIterPair<TIter, TIterEnd>)
        //     {
        //         std::shift_left(&*iter, &*iterEnd, steps);
        //         return;
        //     }

        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::shift_left(stdIter, stdIterEnd, steps);
        // }

        // template <class TIter, class TIterEnd>
        // constexpr auto _BwdRotate(TIter iter, TIterEnd iterEnd, usize steps) const -> void
        // {
        //     if constexpr (RArrIterPair<TIter, TIterEnd>)
        //     {
        //         std::rotate(&*iter, &*iter + steps, &*iterEnd);
        //         return;
        //     }

        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::rotate(stdIter, stdIter + steps, stdIterEnd);
        // }

        // template <class TIter, class TIterEnd>
        // constexpr auto _FwdRotate(TIter iter, TIterEnd iterEnd, isize steps) const -> void
        // {
        //     if constexpr (RArrIterPair<TIter, TIterEnd>)
        //     {
        //         std::rotate(&*iter, &*iterEnd - steps, &*iterEnd);
        //         return;
        //     }

        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::rotate(stdIter, stdIterEnd - steps, stdIterEnd);
        // }

        // template <class TIter, class TIterEnd>
        // constexpr auto _Destruct(TIter iter, TIterEnd iterEnd) const -> void
        // {
        //     if constexpr (RArrIterPair<TIter, TIterEnd>)
        //     {
        //         std::destroy(&*iter, &*iterEnd);
        //         return;
        //     }

        //     StdIterWrapForAtomIter stdIter{ iter };
        //     StdIterWrapForAtomIter stdIterEnd{ iterEnd };

        //     std::destroy(stdIter, stdIterEnd);
        // }
    };

    template <typename T>
    class MutRangeTrait: public RangeTrait<T>
    {
        using Base = RangeTrait<T>;
        using _Impl = _MutRangeTraitImpl<T>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = typename _Impl::TIter;
        using TIterEnd = typename _Impl::TIterEnd;
        using TMutIter = typename _Impl::TMutIter;
        using TMutIterEnd = typename _Impl::TMutIterEnd;

        ////
        ///-------------------------------------------------------------------------------------------
        //// Iteration
        ////
        ///-------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() -> TMutIter
        {
            return _Impl::mutIter();
        }

        using Base::begin;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() -> TMutIterEnd
        {
            return _Impl::mutIterEnd();
        }

        using Base::end;

        ////
        ///-------------------------------------------------------------------------------------------
        ////
        ////
        ///-------------------------------------------------------------------------------------------

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     template <typename T1>
        //     constexpr auto writeElems(T1& val) const -> void
        //         requires(RAssignable<TElem, T1>)
        //     {
        //         _Fill(range.iter(), range.iterEnd(), fwd(val));
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        // #pragma optimize("", off)
        //     template <typename T1>
        //     constexpr auto writeElemsNoOptimize(T1& val) const -> void
        //         requires(RAssignable<TElem, T1>)
        //     {
        //         _Fill(range.iter(), range.iterEnd(), fwd(val));
        //     }

        // #pragma optimize("", on)

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto shiftFwd(usize steps)
        //         requires(RMoveAssignable<TElem>)
        //     {
        //         _FwdShift(range.iter(), range.iterEnd(), steps);
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto shiftBwd(usize steps)
        //         requires(RMoveAssignable<TElem>)
        //     {
        //         _BwdShift(range.iter(), range.iterEnd(), steps);
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto shiftBy(isize steps)
        //         requires(RSwappable<TElem>)
        //     {
        //         if (steps > 0)
        //         {
        //             _BwdShift(range.iter(), range.iterEnd(), steps);
        //         }
        //         else
        //         {
        //             _FwdShift(range.iter(), range.iterEnd(), -steps);
        //         }
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto rotateFwd(usize steps)
        //         requires(RSwappable<TElem>)
        //     {
        //         _FwdRotate(range.iter(), range.iterEnd(), steps);
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto rotateBwd(usize steps)
        //         requires(RSwappable<TElem>)
        //     {
        //         _BwdRotate(range.iter(), range.iterEnd(), steps);
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto rotateBy(isize steps)
        //         requires(RSwappable<TElem>)
        //     {
        //         if (steps > 0)
        //         {
        //             _BwdRotate(range.iter(), range.iterEnd(), steps);
        //         }
        //         else
        //         {
        //             _FwdRotate(range.iter(), range.iterEnd(), -steps);
        //         }
        //     }

        //     ///
        //     ----------------------------------------------------------------------------------------
        //     ///
        //     ///
        //     ----------------------------------------------------------------------------------------
        //     constexpr auto destroyElems()
        //         requires(RDestructible<TElem>)
        //     {
        //         _Destruct(range.iter(), range.iterEnd());
        //     }
    };
}
