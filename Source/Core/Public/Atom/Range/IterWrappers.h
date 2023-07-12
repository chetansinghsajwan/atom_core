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

        constexpr fn operator *() noex -> TElem&
        {
            return &*this->iter;
        }

        constexpr fn operator ->() noex -> TElem*
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

        constexpr ctor IterWrap(TIter iter) noex:
            iter{ iter } { }

        template <tname... TArgs>
        requires RConstructible<TIter, TArgs...>
        constexpr ctor IterWrap(TArgs&&... args) noex:
            iter{ fwd(args)... } { }

        constexpr fn operator *() const noex -> const TElem&
        {
            return *this->iter;
        }

        constexpr fn operator ->() const noex -> const TElem*
        {
            return &*this->iter;
        }

        constexpr fn operator ++(int) noex -> IterWrap&
        {
            this->iter++;
            return *this;
        }

        template <tname TIterEnd>
        constexpr fn operator ==(const IterWrap<TIterEnd>& end) const noex -> bool
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

        constexpr ctor MutIterWrap(TIter iter) noex:
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

        constexpr ctor FwdIterWrap(TIter iter) noex:
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

        constexpr ctor MutFwdIterWrap(TIter iter) noex:
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

        constexpr ctor BidiIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr fn operator --(int) noex -> BidiIterWrap&
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

        constexpr ctor MutBidiIterWrap(TIter iter) noex:
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

        constexpr ctor JumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr fn operator +(isize steps) const noex -> JumpIterWrap
        {
            return JumpIterWrap{ this->iter + steps };
        }

        constexpr fn operator -(isize steps) const noex -> JumpIterWrap
        {
            return JumpIterWrap{ this->iter - steps };
        }

        constexpr fn operator +=(isize steps) noex -> JumpIterWrap&
        {
            this->iter += steps;
            return *this;
        }

        constexpr fn operator -=(isize steps) noex -> JumpIterWrap&
        {
            this->iter -= steps;
            return *this;
        }

        template <tname TIter2>
        constexpr fn operator -(const TIter2& iter2) const noex -> isize
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

        constexpr ctor MutJumpIterWrap(TIter iter) noex:
            Base{ iter } { }

        constexpr fn operator +(isize steps) const noex -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ this->iter + steps };
        }

        constexpr fn operator -(isize steps) const noex -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ this->iter - steps };
        }

        template <tname TIter2>
        constexpr fn operator -(const TIter2& iter2) const noex -> isize
        {
            return this->iter - iter2;
        }
    };
}