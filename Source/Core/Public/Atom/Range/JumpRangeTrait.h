#pragma once
#include "RangeTrait.h"

namespace Atom
{
    template <typename TRange>
    class FwdRangeTrait:
        public RangeTrait<TRange>
    {};

    template <typename TRange>
    class BidiRangeTrait:
        public FwdRangeTrait<TRange>
    {};

    template <typename TRange>
    class JumpRangeTrait:
        public BidiRangeTrait<TRange>
    {};
}