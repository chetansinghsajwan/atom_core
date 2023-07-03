#include "catch2/catch_all.hpp"
#include "Atom/Range/IterWrappers.h"
#include "Atom/Range/ArrIter.h"

using namespace Atom;

TEST_CASE("Atom.Range.IterWrappers", "[.compilation]")
{
    using IterWrapMock = IterWrap<ArrIter<int>>;
    using MutIterWrapMock = MutIterWrap<ArrIter<int>>;
    using FwdIterWrapMock = FwdIterWrap<ArrIter<int>>;
    using MutFwdIterWrapMock = MutFwdIterWrap<ArrIter<int>>;
    using BidiIterWrapMock = BidiIterWrap<ArrIter<int>>;
    using MutBidiIterWrapMock = MutBidiIterWrap<ArrIter<int>>;
    using JumpIterWrapMock = JumpIterWrap<ArrIter<int>>;
    using MutJumpIterWrapMock = MutJumpIterWrap<ArrIter<int>>;

    STATIC_CHECK(RIter<IterWrapMock>);
    STATIC_CHECK_FALSE(RMutIter<IterWrapMock>);
    STATIC_CHECK_FALSE(RFwdIter<IterWrapMock>);

    STATIC_CHECK(RMutIter<MutIterWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdIter<IterWrapMock>);

    STATIC_CHECK(RFwdIter<FwdIterWrapMock>);
    STATIC_CHECK_FALSE(RMutFwdIter<FwdIterWrapMock>);
    STATIC_CHECK_FALSE(RBidiIter<IterWrapMock>);

    STATIC_CHECK(RMutFwdIter<MutFwdIterWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiIter<IterWrapMock>);

    STATIC_CHECK(RBidiIter<BidiIterWrapMock>);
    STATIC_CHECK_FALSE(RMutBidiIter<BidiIterWrapMock>);
    STATIC_CHECK_FALSE(RJumpIter<IterWrapMock>);

    STATIC_CHECK(RMutBidiIter<MutBidiIterWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpIter<IterWrapMock>);

    STATIC_CHECK(RJumpIter<JumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutJumpIter<JumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);

    STATIC_CHECK(RMutJumpIter<MutJumpIterWrapMock>);
    STATIC_CHECK_FALSE(RMutArrIter<IterWrapMock>);
}

TEST_CASE("Atom.Range.IterWrappers", "[.interface]")
{
    struct CustomIter: ArrIter<int>
    {
        CustomIter(int, char, float, const char*) { }
    };

    using IterWrapMock = IterWrap<CustomIter>;
    using MutIterWrapMock = MutIterWrap<CustomIter>;
    using FwdIterWrapMock = FwdIterWrap<CustomIter>;
    using MutFwdIterWrapMock = MutFwdIterWrap<CustomIter>;
    using BidiIterWrapMock = BidiIterWrap<CustomIter>;
    using MutBidiIterWrapMock = MutBidiIterWrap<CustomIter>;
    using JumpIterWrapMock = JumpIterWrap<CustomIter>;
    using MutJumpIterWrapMock = MutJumpIterWrap<CustomIter>;

    IterWrapMock iter{ 0, 'a', 0.0f, "hello" };
    MutIterWrapMock mutIter{ 0, 'a', 0.0f, "hello" };
    FwdIterWrapMock fwdIter{ 0, 'a', 0.0f, "hello" };
    MutFwdIterWrapMock mutFwdIter{ 0, 'a', 0.0f, "hello" };
    BidiIterWrapMock bidiIter{ 0, 'a', 0.0f, "hello" };
    MutBidiIterWrapMock mutBidiIter{ 0, 'a', 0.0f, "hello" };
    JumpIterWrapMock jumpIter{ 0, 'a', 0.0f, "hello" };
    MutJumpIterWrapMock mutJumpIter{ 0, 'a', 0.0f, "hello" };
}