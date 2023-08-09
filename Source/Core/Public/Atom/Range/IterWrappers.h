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
    class _BasicMutIterWrap extends TWrap
    {
        pub using Base = TWrap;
        pub using TElem = typename Base::TElem;

        pub using Base::Base;

        pub using Base::op*;
        pub using Base::op->;

        pub cexpr fn op*() -> TElem&
        {
            return &self->iter;
        }

        pub cexpr fn op->() -> TElem*
        {
            return &self->iter;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TWrap>
    requires (!RIter<TWrap>)
    class _BasicMutIterWrap<TWrap> extends TWrap { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class IterWrap
    {
        pub using TElem = TTI::TEnableIf<RIter<TIter>, typename TIter::TElem>;

        pub cexpr ctor IterWrap(TIter iter):
            iter{ iter } { }

        pub template <typename... TArgs>
        requires RConstructible<TIter, TArgs...>
        cexpr ctor IterWrap(TArgs&&... args):
            iter{ fwd(args)... } { }

        pub cexpr fn op*() const -> const TElem&
        {
            return self->iter;
        }

        pub cexpr fn op->() const -> const TElem*
        {
            return &self->iter;
        }

        pub cexpr fn op++(i32) -> IterWrap&
        {
            self.iter++;
            return self;
        }

        pub template <typename TIterEnd>
        cexpr fn op==(const IterWrap<TIterEnd>& end) const -> bool
        {
            return self.iter == end.iter;
        }

        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires (!RIter<TIter>)
    class IterWrap<TIter>
    {
        TIter iter;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class MutIterWrap extends _BasicMutIterWrap<IterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<IterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    class FwdIterWrap extends IterWrap<TIter>,
        public MultiPassIterTag
    {
        pub using Base = IterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor FwdIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutFwdIter<TIter>
    class MutFwdIterWrap extends _BasicMutIterWrap<FwdIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<FwdIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutFwdIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RBidiIter<TIter>
    class BidiIterWrap extends FwdIterWrap<TIter>
    {
        pub using Base = FwdIterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor BidiIterWrap(TIter iter):
            Base{ iter } { }

        pub cexpr fn op--(i32) -> BidiIterWrap&
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
    class MutBidiIterWrap extends _BasicMutIterWrap<BidiIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<BidiIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutBidiIterWrap(TIter iter):
            Base{ iter } { }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RJumpIter<TIter>
    class JumpIterWrap extends BidiIterWrap<TIter>
    {
        pub using Base = BidiIterWrap<TIter>;
        pub using Base::Base;

        pub cexpr ctor JumpIterWrap(TIter iter):
            Base{ iter } { }

        pub cexpr fn op+(isize steps) const -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter + steps };
        }

        pub cexpr fn op-(isize steps) const -> JumpIterWrap
        {
            return JumpIterWrap{ self.iter - steps };
        }

        pub cexpr fn op+=(isize steps) -> JumpIterWrap&
        {
            self.iter += steps;
            return self;
        }

        pub cexpr fn op-=(isize steps) -> JumpIterWrap&
        {
            self.iter -= steps;
            return self;
        }

        template <typename TIter2>
        pub cexpr fn op-(const TIter2& iter2) const -> isize
        {
            return self.iter - iter2;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    requires RMutJumpIter<TIter>
    class MutJumpIterWrap extends _BasicMutIterWrap<JumpIterWrap<TIter>>
    {
        pub using Base = _BasicMutIterWrap<JumpIterWrap<TIter>>;
        pub using Base::Base;

        pub cexpr ctor MutJumpIterWrap(TIter iter):
            Base{ iter } { }

        pub cexpr fn op+(isize steps) const -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter + steps };
        }

        pub cexpr fn op-(isize steps) const -> MutJumpIterWrap
        {
            return MutJumpIterWrap{ self.iter - steps };
        }

        template <typename TIter2>
        pub cexpr fn op-(const TIter2& iter2) const -> isize
        {
            return self.iter - iter2;
        }
    };
}