// #include "catch2/catch_all.hpp"
// #include "Atom/Range/RangeWrappers.h"
// #include "Atom/Range/Range.h"
//
// using namespace Atom;
//
// template <typename T>
// using MutArrayRange = Range<T*, T*>;
//
// TEST_CASE("Atom.Range.RangeWrappers", "[.compilation]")
// {
//     using RangeWrapMock = RangeRefWrap<MutArrayRange<i32>>;
//     using MutRangeWrapMock = MutRangeRefWrap<MutArrayRange<i32>>;
//     using FwdRangeWrapMock = FwdRangeRefWrap<MutArrayRange<i32>>;
//     using MutFwdRangeWrapMock = MutFwdRangeRefWrap<MutArrayRange<i32>>;
//     using BidiRangeWrapMock = BidiRangeRefWrap<MutArrayRange<i32>>;
//     using MutBidiRangeWrapMock = MutBidiRangeRefWrap<MutArrayRange<i32>>;
//     using JumpRangeWrapMock = JumpRangeRefWrap<MutArrayRange<i32>>;
//     using MutJumpRangeWrapMock = MutJumpRangeRefWrap<MutArrayRange<i32>>;
//
//     STATIC_CHECK(RRange<RangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutRange<RangeWrapMock>);
//     STATIC_CHECK_FALSE(RFwdRange<RangeWrapMock>);
//
//     STATIC_CHECK(RMutRange<MutRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutFwdRange<RangeWrapMock>);
//
//     STATIC_CHECK(RFwdRange<FwdRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutFwdRange<FwdRangeWrapMock>);
//     STATIC_CHECK_FALSE(RBidiRange<RangeWrapMock>);
//
//     STATIC_CHECK(RMutFwdRange<MutFwdRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutBidiRange<RangeWrapMock>);
//
//     STATIC_CHECK(RBidiRange<BidiRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutBidiRange<BidiRangeWrapMock>);
//     STATIC_CHECK_FALSE(RJumpRange<RangeWrapMock>);
//
//     STATIC_CHECK(RMutBidiRange<MutBidiRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutJumpRange<RangeWrapMock>);
//
//     STATIC_CHECK(RJumpRange<JumpRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutJumpRange<JumpRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutArrayRange<RangeWrapMock>);
//
//     STATIC_CHECK(RMutJumpRange<MutJumpRangeWrapMock>);
//     STATIC_CHECK_FALSE(RMutArrayRange<RangeWrapMock>);
// }
//
// TEST_CASE("Atom.Range.RangeWrappers", "[.compilation]")
// {
//     Range<i32*, i32*> range{ nullptr, nullptr };
//
//     RangeRefWrap rangeWrap{ range };
//     MutRangeRefWrap mutRangeWrap{ range };
//     FwdRangeRefWrap fwdRangeWrap{ range };
//     MutFwdRangeRefWrap mutFwdRangeWrap{ range };
//     BidiRangeRefWrap bidiRangeWrap{ range };
//     MutBidiRangeRefWrap mutBidiRangeWrap{ range };
//     JumpRangeRefWrap jumpRangeWrap{ range };
//     MutJumpRangeRefWrap mutJumpRangeWrap{ range };
// }
