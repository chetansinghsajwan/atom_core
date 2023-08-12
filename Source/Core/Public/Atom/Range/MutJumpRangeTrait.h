#pragma once
#include "JumpRangeTrait.h"
#include "MutRangeTrait.h"

namespace Atom
{
    template <typename TRange>
    class MutFwdRangeTrait:
        public FwdRangeTrait<TRange>,
        public MutRangeTrait<TRange>
    {};

    template <typename TRange>
    class MutBidiRangeTrait:
        public BidiRangeTrait<TRange>,
        public MutFwdRangeTrait<TRange>
    {};

    template <typename TRange>
    class MutJumpRangeTrait:
        public JumpRangeTrait<TRange>,
        public MutBidiRangeTrait<TRange>
    {};
}