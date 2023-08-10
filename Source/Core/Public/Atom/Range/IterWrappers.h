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
    class _BasicMutIterWrap : public TWrap
    {
    public:
        using Base = TWrap;
        using TElem = typename Base::TElem;

    public:
        using Base::Base;

    public:
        using Base::operator*;
        using Base::operator->;

        constexpr auto operator*() -> TElem&
        {
            return &self->iter;
        }

        constexpr auto operator->() -> TElem*
        {
            return &self->iter;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TWrap>
    requires (!RIter<TWrap>)
    class _BasicMutIterWrap<TWrap> : public TWrap { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class IterWrap
    {
    public:
        using TElem = TTI::TEnableIf<RIter<TIter>, typename TIter::TElem>;

    public:
        constexpr IterWrap(TIter iter):
            iter{ iter } { }

        template <typename... TArgs>
        requires RConstructible<TIter, TArgs...>
        constexpr IterWrap(TArgs&&... args):
            iter{ fwd(args)... } { }

    public:
        constexpr auto operator*() const -> const TElem&
        {
            return self->iter;
        }

        constexpr auto operator->() const -> const TElem*
        {
            return &self->iter;
        }

        constexpr auto operator++(i32) -> IterWrap&
        {
            self.iter++;
            return self;
        }

        template <typename TIterEnd>
        constexpr auto operator==(const IterWrap<TIterEnd>& end) const -> bool
        {
            return self.iter == end.iter;
        }

    public:
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires (!RIter<TIter>)
    class IterWrap<TIter>
    {
    public:
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class MutIterWrap : public _BasicMutIterWrap<IterWrap<TIter>>
    {
    public:
        using Base = _BasicMutIterWrap<IterWrap<TIter>>;
        using Base::Base;

    public:
        constexpr MutIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class FwdIterWrap : public IterWrap<TIter>,
        public MultiPassIterTag
    {
    public:
        using Base = IterWrap<TIter>;
        using Base::Base;

    public:
        constexpr FwdIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutFwdIter<TIter>
    class MutFwdIterWrap : public _BasicMutIterWrap<FwdIterWrap<TIter>>
    {
    public:
        using Base = _BasicMutIterWrap<FwdIterWrap<TIter>>;
        using Base::Base;

    public:
        constexpr MutFwdIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RBidiIter<TIter>
    class BidiIterWrap : public FwdIterWrap<TIter>
    {
    public:
        using Base = FwdIterWrap<TIter>;
        using Base::Base;

    public:
        constexpr BidiIterWrap(TIter iter):
            Base{ iter } { }

        constexpr auto operator--(i32) -> BidiIterWrap&
        {
            self.iter--;
            return self;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutBidiIter<TIter>
    class MutBidiIterWrap : public _BasicMutIterWrap<BidiIterWrap<TIter>>
    {
    public:
        using Base = _BasicMutIterWrap<BidiIterWrap<TIter>>;
        using Base::Base;

    public:
        constexpr MutBidiIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RJumpIter<TIter>
    class JumpIterWrap : public BidiIterWrap<TIter>
    {
    public:
        using Base = BidiIterWrap<TIter>;
        using Base::Base;

    public:
        constexpr JumpIterWrap(TIter iter):
            Base{ iter } { }

        constexpr auto operator+(isize steps) const -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter + steps };
        }

        constexpr auto operator-(isize steps) const -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter - steps };
        }

        constexpr auto operator+=(isize steps) -> JumpIterWrap&
        {
            self.iter += steps;
            return self;
        }

        constexpr auto operator-=(isize steps) -> JumpIterWrap&
        {
            self.iter -= steps;
            return self;
        }

        template <typename TIter2>
        constexpr auto operator-(const TIter2& iter2) const -> isize
        {
            return self.iter - iter2;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutJumpIter<TIter>
    class MutJumpIterWrap : public _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
    public:
        using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        using Base::Base;

    public:
        constexpr MutJumpIterWrap(TIter iter):
            Base{ iter } { }

        constexpr auto operator+(isize steps) const -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter + steps };
        }

        constexpr auto operator-(isize steps) const -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter - steps };
        }

        template <typename TIter2>
        constexpr auto operator-(const TIter2& iter2) const -> isize
        {
            return self.iter - iter2;
        }
    };
}