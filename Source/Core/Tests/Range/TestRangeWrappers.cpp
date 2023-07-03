#include "catch2/catch_all.hpp"
#include "Atom/Range/RangeWrappers.h"
#include "Atom/Range/Range.h"

using namespace Atom;

template <typename T>
using MutArrRange = Range<T*, T*>;

TEST_CASE("Atom.Range.RangeWrappers", "[.compilation]")
{
    using RangeWrapMock = RangeRefWrap<MutArrRange<int>>;
    using MutRangeWrapMock = MutRangeRefWrap<MutArrRange<int>>;
    using FwdRangeWrapMock = FwdRangeRefWrap<MutArrRange<int>>;
    using MutFwdRangeWrapMock = MutFwdRangeRefWrap<MutArrRange<int>>;
    using BidiRangeWrapMock = BidiRangeRefWrap<MutArrRange<int>>;
    using MutBidiRangeWrapMock = MutBidiRangeRefWrap<MutArrRange<int>>;
    using JumpRangeWrapMock = JumpRangeRefWrap<MutArrRange<int>>;
    using MutJumpRangeWrapMock = MutJumpRangeRefWrap<MutArrRange<int>>;

    STATIC_CHECK(RRange<RangeWrapMock>);
    STATIC_CHECK_FALSE(RMutRange<RangeWrapMock>);
    STATIC_CHECK_FALSE(RFwdRange<RangeWrapMock>);

    STATIC_CHECK(RMutRange<MutRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdRange<RangeWrapMock>);

    STATIC_CHECK(RFwdRange<FwdRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdRange<FwdRangeWrapMock>);
    STATIC_CHECK_FALSE(RBidiRange<RangeWrapMock>);

    STATIC_CHECK(RMutFwdRange<MutFwdRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiRange<RangeWrapMock>);

    STATIC_CHECK(RBidiRange<BidiRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiRange<BidiRangeWrapMock>);
    STATIC_CHECK_FALSE(RJumpRange<RangeWrapMock>);

    STATIC_CHECK(RMutBidiRange<MutBidiRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpRange<RangeWrapMock>);

    STATIC_CHECK(RJumpRange<JumpRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpRange<JumpRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutArrRange<RangeWrapMock>);

    STATIC_CHECK(RMutJumpRange<MutJumpRangeWrapMock>);
    STATIC_CHECK_FALSE(RMutArrRange<RangeWrapMock>);
}

TEST_CASE("Atom.Range.RangeWrappers", "[.compilation]")
{
    Range<int*, int*> range{ nullptr, nullptr };

    RangeRefWrap range{ range };
    MutRangeRefWrap mutRange{ range };
    FwdRangeRefWrap fwdRange{ range };
    MutFwdRangeRefWrap mutFwdRange{ range };
    BidiRangeRefWrap bidiRange{ range };
    MutBidiRangeRefWrap mutBidiRange{ range };
    JumpRangeRefWrap jumpRange{ range };
    MutJumpRangeRefWrap mutJumpRange{ range };
}