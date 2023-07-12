#pragma once
#include "IterReq.h"
#include "IterReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template<tname TWrap>
    struct _BasicMutIterWrap: TWrap
    {
        using Base = TWrap;
        using TElem = tname Base::TElem;

        using Base::Base;

        using Base::operator *;
        using Base::operator ->;

        constexpr TElem& operator *() noex
        {
            return &*this->iter;
        }

        constexpr TElem* operator ->() noex
        {
            return &*this->iter;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TWrap>
    requires (!RIter<TWrap>)
    struct _BasicMutIterWrap<TWrap>: TWrap { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    struct IterWrap
    {
        using TElem = TTI::TEnableIf<RIter<TIter>, tname TIter::TElem>;

        constexpr IterWrap(TIter iter) noex:
            iter{ iter } { }

        template <tname... TArgs>
        requires RConstructible<TIter, TArgs...>
        constexpr IterWrap(TArgs&&... args) noex:
            iter{ FORWARD(args)... } { }

        constexpr const TElem& operator *() const noex
        {
            return *this->iter;
        }

        constexpr const TElem* operator ->() const noex
        {
            return &*this->iter;
        }

        constexpr IterWrap& operator ++(int) noex
        {
            this->iter++;
            return *this;
        }

        template <tname TIterEnd>
        constexpr bool operator ==(const IterWrap<TIterEnd>& end) const noex
        {
            return this->iter == end.iter;
        }

        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires (!RIter<TIter>)
    struct IterWrap<TIter>
    {
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    struct MutIterWrap: _BasicMutIterWrap<IterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<IterWrap<TIter>>;
        using Base::Base;

        constexpr MutIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    struct FwdIterWrap: IterWrap<TIter>, MultiPassIterTag
    {
        using Base = IterWrap<TIter>;
        using Base::Base;

        constexpr FwdIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutFwdIter<TIter>
    struct MutFwdIterWrap: _BasicMutIterWrap<FwdIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<FwdIterWrap<TIter>>;
        using Base::Base;

        constexpr MutFwdIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RBidiIter<TIter>
    struct BidiIterWrap: FwdIterWrap<TIter>
    {
        using Base = FwdIterWrap<TIter>;
        using Base::Base;

        constexpr BidiIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr BidiIterWrap& operator --(int) noex
        {
            this->iter--;
            return *this;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutBidiIter<TIter>
    struct MutBidiIterWrap: _BasicMutIterWrap<BidiIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<BidiIterWrap<TIter>>;
        using Base::Base;

        constexpr MutBidiIterWrap(TIter iter) noex:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RJumpIter<TIter>
    struct JumpIterWrap: BidiIterWrap<TIter>
    {
        using Base = BidiIterWrap<TIter>;
        using Base::Base;

        constexpr JumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr JumpIterWrap operator +(isize steps) const noex
        {
            return JumpIterWrap{ this->iter + steps };
        }

        constexpr JumpIterWrap operator -(isize steps) const noex
        {
            return JumpIterWrap{ this->iter - steps };
        }

        constexpr JumpIterWrap& operator +=(isize steps) noex
        {
            this->iter += steps;
            return *this;
        }

        constexpr JumpIterWrap& operator -=(isize steps) noex
        {
            this->iter -= steps;
            return *this;
        }

        template <tname TIter2>
        constexpr isize operator -(const TIter2& iter2) const noex
        {
            return this->iter - iter2;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    requires RMutJumpIter<TIter>
    struct MutJumpIterWrap: _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        using Base::Base;

        constexpr MutJumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr MutJumpIterWrap operator +(isize steps) const noex
        {
            return MutJumpIterWrap{ this->iter + steps };
        }

        constexpr MutJumpIterWrap operator -(isize steps) const noex
        {
            return MutJumpIterWrap{ this->iter - steps };
        }

        template <tname TIter2>
        constexpr isize operator -(const TIter2& iter2) const noex
        {
            return this->iter - iter2;
        }
    };
}