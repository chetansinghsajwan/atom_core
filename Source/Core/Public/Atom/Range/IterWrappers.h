#pragma once
#include "IterReq.h"
#include "IterReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template<typename TWrap>
    struct _BasicMutIterWrap: TWrap
    {
        using Base = TWrap;
        using TElem = typename Base::TElem;

        using Base::Base;

        using Base::operator *;
        using Base::operator ->;

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

        constexpr IterWrap(TIter iter) noexcept:
            iter{ iter } { }

        template <typename... TArgs>
        requires RConstructible<TIter, TArgs...>
        constexpr IterWrap(TArgs&&... args) noexcept:
            iter{ FORWARD(args)... } { }

        constexpr const TElem& operator *() const noexcept
        {
            return *this->iter;
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
    struct MutIterWrap: _BasicMutIterWrap<IterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<IterWrap<TIter>>;
        using Base::Base;

        constexpr MutIterWrap(TIter iter) noexcept:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    struct FwdIterWrap: IterWrap<TIter>, MultiPassIterTag
    {
        using Base = IterWrap<TIter>;
        using Base::Base;

        constexpr FwdIterWrap(TIter iter) noexcept:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutFwdIter<TIter>
    struct MutFwdIterWrap: _BasicMutIterWrap<FwdIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<FwdIterWrap<TIter>>;
        using Base::Base;

        constexpr MutFwdIterWrap(TIter iter) noexcept:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RBidiIter<TIter>
    struct BidiIterWrap: FwdIterWrap<TIter>
    {
        using Base = FwdIterWrap<TIter>;
        using Base::Base;

        constexpr BidiIterWrap(TIter iter) noexcept:
            Base{ iter } { }

        constexpr BidiIterWrap& operator --(int) noexcept
        {
            this->iter--;
            return *this;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutBidiIter<TIter>
    struct MutBidiIterWrap: _BasicMutIterWrap<BidiIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<BidiIterWrap<TIter>>;
        using Base::Base;

        constexpr MutBidiIterWrap(TIter iter) noexcept:
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RJumpIter<TIter>
    struct JumpIterWrap: BidiIterWrap<TIter>
    {
        using Base = BidiIterWrap<TIter>;
        using Base::Base;

        constexpr JumpIterWrap(TIter iter) noexcept:
            Base{ iter } { }

        constexpr JumpIterWrap operator +(isize steps) const noexcept
        {
            return JumpIterWrap{ this->iter + steps };
        }

        constexpr JumpIterWrap operator -(isize steps) const noexcept
        {
            return JumpIterWrap{ this->iter - steps };
        }

        constexpr JumpIterWrap& operator +=(isize steps) noexcept
        {
            this->iter += steps;
            return *this;
        }

        constexpr JumpIterWrap& operator -=(isize steps) noexcept
        {
            this->iter -= steps;
            return *this;
        }

        template <typename TIter2>
        constexpr isize operator -(const TIter2& iter2) const noexcept
        {
            return this->iter - iter2;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutJumpIter<TIter>
    struct MutJumpIterWrap: _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        using Base::Base;

        constexpr MutJumpIterWrap(TIter iter) noexcept:
            Base{ iter } { }

        constexpr MutJumpIterWrap operator +(isize steps) const noexcept
        {
            return MutJumpIterWrap{ this->iter + steps };
        }

        constexpr MutJumpIterWrap operator -(isize steps) const noexcept
        {
            return MutJumpIterWrap{ this->iter - steps };
        }

        template <typename TIter2>
        constexpr isize operator -(const TIter2& iter2) const noexcept
        {
            return this->iter - iter2;
        }
    };
}