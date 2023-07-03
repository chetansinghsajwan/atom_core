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
    struct _BasicMutIterWrap: TWrap
    {
        using TElem = typename TWrap::TElem;

        using TWrap::TWrap;

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
    template <typename TWrap>
    requires (!RIter<TWrap>)
    struct _BasicMutIterWrap<TWrap>: TWrap { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct IterWrap
    {
        using TElem = TTI::TEnableIf<RIter<TIter>, typename TIter::TElem>;

        template <typename... TArgs>
        requires RConstructible<TIter, TArgs...>
        constexpr IterWrap(TArgs&&... args) noexcept:
            iter{ FORWARD(args)... } { }

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
        constexpr bool operator ==(const IterWrap<TIterEnd>& end) const noexcept
        {
            return this->iter == end.iter;
        }

        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires (!RIter<TIter>)
    struct IterWrap<TIter>
    {
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using MutIterWrap = _BasicMutIterWrap<IterWrap<TIter>>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct FwdIterWrap: IterWrap<TIter>, MultiPassIterTag
    {
        using Base = IterWrap<TIter>;
        using Base::Base;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using MutFwdIterWrap = _BasicMutIterWrap<FwdIterWrap<TIter>>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct BidiIterWrap: FwdIterWrap<TIter>
    {
        using Base = FwdIterWrap<TIter>;
        using Base::Base;

        constexpr BidiIterWrap& operator --(int) noexcept
        requires RBidiIter<TIter>
        {
            this->iter--;
            return *this;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    using MutBidiIterWrap = _BasicMutIterWrap<BidiIterWrap<TIter>>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct JumpIterWrap: BidiIterWrap<TIter>
    {
        using Base = BidiIterWrap<TIter>;
        using Base::Base;

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

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct MutJumpIterWrap: _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        using Base::Base;

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
}