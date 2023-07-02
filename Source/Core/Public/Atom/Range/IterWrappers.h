#pragma once
#include "IterReq.h"
#include "IterReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TWrap>
    struct _BasicMutIterWrap: TWrap { };

    template <typename TWrap>
    requires RIter<TWrap>
    struct _BasicMutIterWrap<TWrap>: TWrap
    {
        using TElem = typename TWrap::TElem;

        using TWrap::operator *;
        using TWrap::operator ->;

        constexpr TElem& operator *() noexcept
        {
            return &*this->iter;
        }

        constexpr TElem* operator ->() noexcept
        {
            return &*this->iter;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct IterWrap
    {
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RIter<TIter>
    struct IterWrap<TIter>
    {
        using TElem = typename TIter::TElem;

        constexpr const TElem& operator *() const noexcept
        {
            return &*this->iter;
        }

        constexpr const TElem* operator ->() const noexcept
        {
            return &*this->iter;
        }

        constexpr IterWrap& operator ++(int) noexcept
        {
            this->iter++;
            return *this;
        }

        template <typename TIterEnd>
        constexpr bool operator ==(TIterEnd end) const noexcept
        {
            return this->iter == end;
        }

        TIter iter;
    };

    template <typename T>
    using _IterWrapTestMock = IterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_ITER_TEMP(_IterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct MutIterWrap: _BasicMutIterWrap<IterWrap<TIter>> { };

    template <typename T>
    using _MutIterWrapTestMock = MutIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_MUT_ITER_TEMP(_MutIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct FwdIterWrap: IterWrap<TIter>, MultiPassIterTag { };

    template <typename T>
    using _FwdIterWrapTestMock = FwdIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_FWD_ITER_TEMP(_FwdIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct MutFwdIterWrap: _BasicMutIterWrap<FwdIterWrap<TIter>> { };

    template <typename T>
    using _MutFwdIterWrapTestMock = MutFwdIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_MUT_FWD_ITER_TEMP(_MutFwdIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct BidiIterWrap: FwdIterWrap<TIter>
    {
        constexpr BidiIterWrap& operator --(int) noexcept
        requires RBidiIter<TIter>
        {
            this->iter--;
            return *this;
        }
    };

    template <typename T>
    using _BidiIterWrapTestMock = BidiIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_BIDI_ITER_TEMP(_BidiIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct MutBidiIterWrap: _BasicMutIterWrap<BidiIterWrap<TIter>> { };

    template <typename T>
    using _MutBidiIterWrapTestMock = MutBidiIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_MUT_BIDI_ITER_TEMP(_MutBidiIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct JumpIterWrap: BidiIterWrap<TIter>
    {
        constexpr JumpIterWrap operator +(isize steps) const noexcept
        requires RJumpIter<TIter>
        {
            return JumpIterWrap{ this->iter + steps };
        }

        constexpr JumpIterWrap operator -(isize steps) const noexcept
        requires RJumpIter<TIter>
        {
            return JumpIterWrap{ this->iter - steps };
        }

        constexpr JumpIterWrap& operator +=(isize steps) noexcept
        requires RJumpIter<TIter>
        {
            this->iter += steps;
            return *this;
        }

        constexpr JumpIterWrap& operator -=(isize steps) noexcept
        requires RJumpIter<TIter>
        {
            this->iter -= steps;
            return *this;
        }

        template <typename TIter2>
        constexpr isize operator -(const TIter2& iter2) const noexcept
        requires RJumpIter<TIter>
        {
            return this->iter - iter2;
        }
    };

    template <typename T>
    using _JumpIterWrapTestMock = JumpIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_JUMP_ITER_TEMP(_JumpIterWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct MutJumpIterWrap: _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        constexpr MutJumpIterWrap operator +(isize steps) const noexcept
        requires RJumpIter<TIter>
        {
            return MutJumpIterWrap{ this->iter + steps };
        }

        constexpr MutJumpIterWrap operator -(isize steps) const noexcept
        requires RJumpIter<TIter>
        {
            return MutJumpIterWrap{ this->iter - steps };
        }

        template <typename TIter2>
        constexpr isize operator -(const TIter2& iter2) const noexcept
        requires RJumpIter<TIter>
        {
            return this->iter - iter2;
        }
    };

    template <typename T>
    using _MutJumpIterWrapTestMock = MutJumpIterWrap<MutArrIterReqMock<T>>;

    ATOM_SATISFIES_MUT_JUMP_ITER_TEMP(_MutJumpIterWrapTestMock);
}